#include "creact.h"

EM_JS(void, creact_render_component,
      (const char *html_tag, const char *html_parent,
       creact_component_prop *props, size_t props_len),
      {
        const htmlTag = UTF8ToString(html_tag);
        const htmlParent = UTF8ToString(html_parent);

        let tagProps = [];
        for (let i = 0; i < props_len; i++) {
          const structPtr = props + i * props_len;

          const namePtr = getValue(structPtr, "*");
          const valuePtr = getValue(structPtr + 4, "*");

          const name = UTF8ToString(namePtr);
          const value = UTF8ToString(valuePtr);

          tagProps.push({name, value});
        }

        const parent = document.getElementById(htmlParent);
        const element = document.createElement(htmlTag);
        for (const prop of tagProps) {
          element.setAttribute(prop.name, prop.value);
        }
        parent.appendChild(element);
      });

creact_element *
creact_create_html_element(const char *key, const char *html_tag,
                           creact_component_prop *props, size_t props_len,
                           creact_element **children, size_t children_len) {
  creact_element *element = malloc(sizeof(creact_element));
  element->type = creact_element_type_html;
  element->html_tag = html_tag;
  element->props = props;
  element->props_len = props_len;
  element->children = children;
  element->children_len = children_len;
  element->key = key;
  return element;
}

creact_element *creact_create_component_element(
    const char *key, creact_component *component, creact_component_prop *props,
    size_t props_len, creact_element **children, size_t children_len) {
  creact_element *element = malloc(sizeof(creact_element));
  element->type = creact_element_type_function;
  element->component = component;
  element->props = props;
  element->props_len = props_len;
  element->children = children;
  element->children_len = children_len;
  element->key = key;
  return element;
}

void creact_render(const char *html_root, creact_element *component) {
  if (component->type == creact_element_type_html) {
    creact_render_component(component->html_tag, html_root, component->props,
                            component->props_len);

    for (size_t i = 0; i < component->children_len; i++) {
      creact_render(component->key, component->children[i]);
    }
  }
}
