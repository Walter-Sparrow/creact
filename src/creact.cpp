#include "creact.h"
#include <cstddef>
#include <cstdio>
#include <emscripten.h>

creact_node creact_create_element(const char *text) {
  creact_node node;
  node.type = creact_element_type_text;
  node.element.text = text;
  return node;
}

creact_node creact_create_element(const char *key, const char *html_tag,
                                  creact_component_props props,
                                  creact_component_children children) {
  creact_node node;
  node.type = creact_element_type_html;
  node.element.html =
      (creact_element_html *)malloc(sizeof(creact_element_html));
  node.element.html->key = key;
  node.element.html->html_tag = html_tag;
  node.element.html->props = props;
  node.element.html->children = children;
  return node;
}

creact_node creact_create_element(creact_function_component component,
                                  creact_component_props props,
                                  creact_component_children children) {
  creact_node node;
  node.type = creact_element_type_function_component;
  node.element.function_component = (creact_element_function_component *)malloc(
      sizeof(creact_element_function_component));
  node.element.function_component->component = component;
  node.element.function_component->props = props;
  node.element.function_component->children = children;
  return node;
}

EM_JS(void, creact_render_component,
      (const char *html_tag, const char *html_parent,
       creact_component_prop *props, size_t props_len),
      {
        const htmlTag = UTF8ToString(html_tag);
        const htmlParent = UTF8ToString(html_parent);

        let tagProps = [];
        for (let i = 0; i < props_len; i++) {
          const structPtr = props + i * 8; // sizeof(creact_component_prop)

          const namePtr = getValue(structPtr, "*");
          const valuePtr = getValue(structPtr + 4, "*");

          const name = UTF8ToString(namePtr);
          const value = UTF8ToString(valuePtr);
          console.log(name, value);

          tagProps.push({name, value});
        }

        const parent = document.getElementById(htmlParent);
        const element = document.createElement(htmlTag);
        for (const prop of tagProps) {
          element.setAttribute(prop.name, prop.value);
        }
        parent.appendChild(element);
      });

EM_JS(void, creact_render_text, (const char *html_parent, const char *text), {
  const htmlParent = UTF8ToString(html_parent);
  const elementText = UTF8ToString(text);
  const parent = document.getElementById(htmlParent);
  const element = document.createTextNode(elementText);
  parent.appendChild(element);
});

void creact_render(const char *html_root, creact_node node) {
  if (node.type == creact_element_type_text) {
    creact_render_text(html_root, node.element.text);
  } else if (node.type == creact_element_type_function_component) {
    creact_node computedNode = node.element.function_component->component(
        node.element.function_component->props,
        node.element.function_component->children);
    creact_render(html_root, computedNode);
  } else if (node.type == creact_element_type_html) {
    creact_render_component(node.element.html->html_tag, html_root,
                            node.element.html->props.values,
                            node.element.html->props.len);
    for (size_t i = 0; i < node.element.html->children.len; i++) {
      creact_render(node.element.html->key,
                    node.element.html->children.values[i]);
    }
  }
}
