#include "creact_callback.h"

static int next_callback_id = 1;
static std::unordered_map<callback_id, std::function<void(std::any)>>
    callback_registry;

extern "C" EMSCRIPTEN_KEEPALIVE void callRegisteredCallback(int id) {
  auto it = callback_registry.find(id);
  if (it != callback_registry.end()) {
    it->second({});
  }
}

emscripten::val
creact_create_js_callback(std::function<void(std::any)> callback) {
  callback_id id = next_callback_id++;
  callback_registry[id] = callback;

  std::string jsCode = R"(
    (function(callbackId) {
        var callRegCb = window.Module._callRegisteredCallback;
        return function() {
            console.log('callRegisteredCallback: ', callbackId);
            callRegCb(callbackId);
        };
    })
  )";

  emscripten::val jsFuncFactory = emscripten::val::global("eval")(jsCode);
  return jsFuncFactory(id);
}
