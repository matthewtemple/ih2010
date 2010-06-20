#include "ih/search/searchey.h"

void ih_search_searchey_init(ih_search_searchey_t *searchey,
    ih_search_searchey_create_f create, ih_search_searchey_destroy_f destroy,
    ih_search_searchey_get_solutions_copy_f get_solutions_copy,
    ih_search_searchey_search_f search)
{
  assert(searchey);

  searchey->create = create;
  searchey->destroy = destroy;
  searchey->get_solutions_copy = get_solutions_copy;
  searchey->search = search;
}
