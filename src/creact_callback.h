#pragma once

#include <any>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include <functional>

using callback_id = int;

#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE void callRegisteredCallback(int id);

#ifdef __cplusplus
}
#endif

emscripten::val
creact_create_js_callback(std::function<void(std::any)> callback);
