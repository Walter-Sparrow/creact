#include "creact.h"
#include <cstddef>
#include <cstdio>
#include <emscripten.h>
#include <emscripten/val.h>
#include <string>
#include <vector>

creact_node creact_create_element(const std::string text) {
  creact_node node;
  node.type = creact_element_type_text;
  node.element = text;
  return node;
}

creact_node creact_create_element(const std::string html_tag,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children) {
  creact_node node;
  node.type = creact_element_type_html;
  creact_element_html element;

  element.html_tag = html_tag;
  element.props = props;
  element.children = children;

  node.element = element;
  return node;
}

creact_node creact_create_element(creact_function_component component,
                                  std::vector<creact_component_prop> props,
                                  std::vector<creact_node> children) {
  creact_node node;
  node.type = creact_element_type_function_component;
  creact_element_function_component element;

  element.component = component;
  element.props = props;
  element.children = children;

  node.element = element;
  return node;
}

EM_JS(emscripten::EM_VAL, creact_render_component,
      (const char *html_tag, emscripten::EM_VAL html_parent,
       creact_component_prop *props, size_t props_len),
      {
        const htmlTag = UTF8ToString(html_tag);
        const parent = Emval.toValue(html_parent);

        let tagProps = [];
        for (let i = 0; i < props_len; i++) {
          const structPtr = props + i * 8; // sizeof(creact_component_prop)

          const namePtr = getValue(structPtr, "*");
          const valuePtr = getValue(structPtr + 4, "*");

          const name = UTF8ToString(namePtr);
          const value = UTF8ToString(valuePtr);

          tagProps.push({name, value});
        }

        const element = document.createElement(htmlTag);
        for (const prop of tagProps) {
          element.setAttribute(prop.name, prop.value);
        }
        parent.appendChild(element);

        return Emval.toHandle(element);
      });

EM_JS(void, creact_render_text,
      (emscripten::EM_VAL html_parent, const char *text), {
        const parent = Emval.toValue(html_parent);
        const elementText = UTF8ToString(text);
        const element = document.createTextNode(elementText);
        parent.appendChild(element);
      });

void creact_render(const std::string html_root, creact_node node) {
  emscripten::val document = emscripten::val::global("document");
  emscripten::val root = document.call<emscripten::val>(
      "getElementById", emscripten::val(html_root));

  creact_render(root, node);
}

void creact_render(emscripten::val html_root, creact_node node) {
  if (node.type == creact_element_type_text) {
    creact_render_text(html_root.as_handle(),
                       std::get<creact_text>(node.element).c_str());
  } else if (node.type == creact_element_type_function_component) {
    creact_element_function_component element =
        std::get<creact_element_function_component>(node.element);
    creact_node computedNode =
        element.component(element.props, element.children);
    creact_render(html_root, computedNode);
  } else if (node.type == creact_element_type_html) {
    creact_element_html element = std::get<creact_element_html>(node.element);
    emscripten::EM_VAL stateNode =
        creact_render_component(element.html_tag.c_str(), html_root.as_handle(),
                                element.props.data(), element.props.size());

    element.stateNode = emscripten::val::take_ownership(stateNode);
    for (size_t i = 0; i < element.children.size(); i++) {
      creact_render(element.stateNode, element.children[i]);
    }
  }
}
