#pragma once

#include <emscripten.h>

typedef enum : char {
  creact_element_type_html = 0,
  creact_element_type_function,
} creact_element_type;

typedef struct {
  const char *name;
  const char *value;
} creact_component_prop;

typedef struct creact_element *(creact_component)(creact_component_prop *props);

typedef struct creact_element {
  creact_element_type type;
  union {
    const char *html_tag;
    creact_component *component;
  };
  creact_component_prop *props;
  size_t props_len;
  struct creact_element **children;
  size_t children_len;
  const char *key;
} creact_element;

void creact_render_component(const char *html_tag, const char *html_parent,
                             creact_component_prop *props, size_t props_len);

creact_element *
creact_create_html_element(const char *key, const char *html_tag,
                           creact_component_prop *props, size_t props_len,
                           creact_element **children, size_t children_len);

creact_element *
creact_create_component_element(const char *key, creact_component *component,
                                creact_component_prop *props, size_t props_len,
                                creact_element **children, size_t children_len);

void creact_render(const char *html_root, creact_element *component);
