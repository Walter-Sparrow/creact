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

typedef struct {
  creact_element_type type;
  union {
    const char *html_tag;
    creact_component *component;
  };
  creact_component_prop *props;
  struct creact_element *children;
  const char *key;
} creact_element;

EM_JS(void, creact_render_component,
      (const char *html_tag, const char *html_parent,
       creact_component_prop **props, int props_len),
      {
        const htmlTag = UTF8ToString(html_tag);
        let props = [];
        for (let i = 0; i < props_len; i++) {
          const structPtr = props + i * props_len;

          const namePtr = getValue(structPtr, "*");
          const valuePtr = getValue(structPtr + 4, "*");

          const name = UTF8ToString(namePtr);
          const value = UTF8ToString(valuePtr);

          props.push({name, value});
        }

        const parent = document.getElementById(html_parent);
        const element = document.createElement(htmlTag);
        for (const prop of props) {
          const prop = props[i];
          element.setAttribute(prop.name, prop.value);
        }
        parent.appendChild(element);
      });

creact_element *creact_create_html_element(const char *key,
                                           const char *html_tag,
                                           creact_component_prop *props,
                                           creact_element *children);

creact_element *creact_create_component_element(const char *key,
                                                creact_component *component,
                                                creact_component_prop *props,
                                                creact_element *children);

void creact_render(const char *html_root, creact_element *component);
