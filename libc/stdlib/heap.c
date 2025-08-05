#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// NOTE: The heap's max size is 1 MB
extern char __heap_start;
extern char __heap_end;

static void *heap_end = &__heap_end;
static void *heap_start = &__heap_start;
static size_t heap_min_size;
static size_t heap_size;
#define heap_max_size (size_t) ((uint8_t *) heap_end - (uint8_t *) heap_start)
#define heap_top ((void *) ((char *) heap_start + heap_size))

typedef struct header {
	size_t size;
	bool is_free;
	struct header *next;
	struct header *prev;
} header_t;

static header_t *first_block;
static header_t *last_block;
static bool heap_is_init;

#if defined(__is_libk)
int heap_initialize(size_t _heap_min_size) {
	if (_heap_min_size > heap_max_size || _heap_min_size < sizeof(header_t) + 1) {
		return -1;
	}
	heap_min_size = _heap_min_size;
	heap_size = heap_min_size;
	first_block = (header_t*) heap_start;
	first_block->size = heap_size - sizeof(header_t);
	first_block->is_free = true;
	first_block->next = NULL;
	first_block->prev = NULL;
	last_block = first_block;
	heap_is_init = true;
	return 0;
}
#endif

static void *sbrk(size_t increment) {
	if (!heap_is_init) return NULL;
	if ((increment + sizeof(header_t) + (uint8_t *) heap_top) >= ((uint8_t *) heap_end)) return NULL;
	size_t new_block_size = sizeof(header_t) + increment;
	if ((uint8_t *) heap_top + new_block_size > (uint8_t *) heap_end) {
		return NULL; // OoB Error
	}
	
	header_t *new_block = (header_t *) heap_top;
	new_block->size = increment;
	new_block->is_free = false;
	new_block->next = NULL;
	new_block->prev = last_block;

	if (last_block) last_block->next = new_block;
	if (!last_block) first_block = new_block; // The heap is empty
	last_block = new_block;
	heap_size += new_block_size;
	return (void *) (new_block + 1);
}

void *malloc(size_t size) {
	if (!heap_is_init) return NULL; // init the heap first lmao
	header_t *current_block = first_block;
	while (current_block) {
		if (current_block->is_free && current_block->size >= size) {
			// We have a winner!
			// WELCOME TO POINTER HELL
			if ((current_block->size - size) >= sizeof(header_t) + 1) {
				header_t *new_block = (header_t *) ((uint8_t *)
				current_block + sizeof(header_t) + size);
				new_block->size = current_block->size - size - sizeof(header_t);
				new_block->is_free = true;
				new_block->next = current_block->next;
				new_block->prev = current_block;

				if (!(new_block->next)) last_block = new_block;

				if (current_block->next) current_block->next->prev = new_block;
				current_block->next = new_block;
			}

			current_block->is_free = false;
			current_block->size = size;
			return (void *) (current_block + 1);
		} else {
			current_block = current_block->next;
		}
	}

	// Current heap isn't suitable. Let's E X P A N D the heap
	void *new_mem = sbrk(size);
	if (new_mem) {
		// Great, we have a new block
		header_t *sbrk_block = (header_t *)new_mem;
		// We don't need to set is_free, as sbrk did that.
		sbrk_block->size = size;
		return (void *) (sbrk_block + 1);
	}

	// No more heap memory :( (probably heapspam, call free())
	return NULL;
}

int free(void *ptr) {
	if (!heap_is_init) return -1;
	// check if this is a valid block
	if (!ptr) return -1; // Failure
	header_t *freed_block = ((header_t *) ptr) - 1;
	if ((void*)freed_block < heap_start || (void*)freed_block >= heap_end) {
		return -1;
	} if (((uintptr_t) freed_block) % __alignof__(header_t) != 0) return -1;
	if (freed_block->is_free || freed_block->size > heap_size || freed_block->size < 1 ||
		(freed_block->next && (uint8_t *) freed_block->next < (uint8_t *) heap_start) ||
		(freed_block->prev && (uint8_t *) freed_block->prev > (uint8_t *) heap_end)) return -1;
	header_t *current_block = first_block;
	bool found = false;
	while (current_block) {
		if (current_block == freed_block) {
			found = true;
			break;
		}
		current_block = current_block->next;
	} if (!found) return -1; // This is all to prevent programs from breaking the heap with fake headers

	freed_block->is_free = true;
	if (freed_block->next) {
		if (freed_block->next->is_free) {
			// Merge this block and the next
			freed_block->size += freed_block->next->size + sizeof(header_t);
			freed_block->next = freed_block->next->next;
			if (freed_block->next) freed_block->next->prev = freed_block;
			// This dereferences the block after this, practically removing it from the heap.
			// We know that the block after this is used, so we don't need to bother about that.
			if (!(freed_block->next)) last_block = freed_block;
		}
	}
	if (freed_block->prev) {
		if (freed_block->prev->is_free) {
			// Merge this block (and possibly the next) with the previous
			freed_block->prev->size += freed_block->size + sizeof(header_t);
			freed_block->prev->next = freed_block->next;
			if (freed_block->next) freed_block->next->prev = freed_block->prev;
			// See the code for the ->next merge check for more info
			// That was painful to write.
			if (!(freed_block->prev->next)) last_block = freed_block->prev;
		}
	}

	return 0;
}

void *calloc(size_t nmemb, size_t size) {
	size_t compat_nmemb = nmemb != 0 ? nmemb : 1;
	size_t compat_size = size != 0 ? size : 1;
	size_t total = compat_nmemb * compat_size;
	if (total >= heap_max_size - heap_size) return NULL;
	if (compat_nmemb != 0 && total / compat_nmemb != compat_size) return NULL;
	void *ptr = malloc(total);
	if (ptr) {
		memset(ptr, 0, total);
	}
	return ptr;
}

void *realloc(void *ptr, size_t new_size) {
	if (!ptr) return malloc(new_size);
	if (new_size == 0) {free(ptr); return NULL;} // We assume realloc(0) means "free the block"
	header_t *hdr = ((header_t*)ptr) - 1;
	if (new_size == hdr->size) return ptr; // We don't need to do extra work.

	if (new_size > hdr->size) {
		header_t *next = hdr->next;

		if (next && next->is_free && (hdr->size + sizeof(header_t) + next->size) >= new_size) {
			hdr->size += sizeof(header_t) + next->size;
			hdr->next = next->next;
			if (hdr->next) hdr->next->prev = hdr;

			size_t excess_size = hdr->size - new_size;
			if (excess_size >= sizeof(header_t) + 1) {
				header_t *new_block = (header_t *) ((uint8_t *) ptr + new_size);
				new_block->size = excess_size - sizeof(header_t);
				new_block->is_free = true;
				new_block->next = hdr->next;
				new_block->prev = hdr;

				if (hdr->next) hdr->next->prev = new_block;
				hdr->next = new_block;
				hdr->size = new_size;
			}

			hdr->is_free = false;
			return ptr;
		}
	}

	void *new_ptr = malloc(new_size);
	if (!new_ptr) return NULL;
	size_t copy_size = hdr->size < new_size ? hdr->size : new_size;
	memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return new_ptr;
}

void print_heap() {
	header_t *current_block = first_block;
	while (current_block) {
		printf("Block %p size %zu is_free %d next %p prev %p\n", current_block, current_block->size, current_block->is_free, current_block->next, current_block->prev);
		current_block = current_block->next;
	}
	printf("\n");
	return;
}