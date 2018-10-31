#include "jsrt/util.h"

#include "core/logger.h"

std::string errorCodeToString(int code) {
   switch (code) {
    case JsNoError:                            return "JsNoError";
    case JsErrorCategoryUsage:                 return "JsErrorCategoryUsage";
    case JsErrorInvalidArgument:               return "JsErrorInvalidArgument";
    case JsErrorNullArgument:                  return "JsErrorNullArgument";
    case JsErrorNoCurrentContext:              return "JsErrorNoCurrentContext";
    case JsErrorInExceptionState:              return "JsErrorInExceptionState";
    case JsErrorNotImplemented:                return "JsErrorNotImplemented";
    case JsErrorWrongThread:                   return "JsErrorWrongThread";
    case JsErrorRuntimeInUse:                  return "JsErrorRuntimeInUse";
    case JsErrorBadSerializedScript:           return "JsErrorBadSerializedScript";
    case JsErrorInDisabledState:               return "JsErrorInDisabledState";
    case JsErrorCannotDisableExecution:        return "JsErrorCannotDisableExecution";
    case JsErrorHeapEnumInProgress:            return "JsErrorHeapEnumInProgress";
    case JsErrorArgumentNotObject:             return "JsErrorArgumentNotObject";
    case JsErrorInProfileCallback:             return "JsErrorInProfileCallback";
    case JsErrorInThreadServiceCallback:       return "JsErrorInThreadServiceCallback";
    case JsErrorCannotSerializeDebugScript:    return "JsErrorCannotSerializeDebugScript";
    case JsErrorAlreadyDebuggingContext:       return "JsErrorAlreadyDebuggingContext";
    case JsErrorAlreadyProfilingContext:       return "JsErrorAlreadyProfilingContext";
    case JsErrorIdleNotEnabled:                return "JsErrorIdleNotEnabled";
    case JsCannotSetProjectionEnqueueCallback: return "JsCannotSetProjectionEnqueueCallback";
    case JsErrorCannotStartProjection:         return "JsErrorCannotStartProjection";
    case JsErrorInObjectBeforeCollectCallback: return "JsErrorInObjectBeforeCollectCallback";
    case JsErrorObjectNotInspectable:          return "JsErrorObjectNotInspectable";
    case JsErrorPropertyNotSymbol:             return "JsErrorPropertyNotSymbol";
    case JsErrorPropertyNotString:             return "JsErrorPropertyNotString";
    case JsErrorInvalidContext:                return "JsErrorInvalidContext";
    case JsInvalidModuleHostInfoKind:          return "JsInvalidModuleHostInfoKind";
    case JsErrorModuleParsed:                  return "JsErrorModuleParsed";
    // JsErrorCategoryEngine
    case JsErrorCategoryEngine:                return "JsErrorCategoryEngine";
    case JsErrorOutOfMemory:                   return "JsErrorOutOfMemory";
    case JsErrorBadFPUState:                   return "JsErrorBadFPUState";
    // JsErrorCategoryScript
    case JsErrorCategoryScript:                return "JsErrorCategoryScript";
    case JsErrorScriptException:               return "JsErrorScriptException";
    case JsErrorScriptCompile:                 return "JsErrorScriptCompile";
    case JsErrorScriptTerminated:              return "JsErrorScriptTerminated";
    case JsErrorScriptEvalDisabled:            return "JsErrorScriptEvalDisabled";
    // JsErrorCategoryFatal
    case JsErrorCategoryFatal:                 return "JsErrorCategoryFatal";
    case JsErrorFatal:                         return "JsErrorFatal";
    case JsErrorWrongRuntime:                  return "JsErrorWrongRuntime";
    // JsErrorCategoryDiagError
    case JsErrorCategoryDiagError:             return "JsErrorCategoryDiagError";
    case JsErrorDiagAlreadyInDebugMode:        return "JsErrorDiagAlreadyInDebugMode";
    case JsErrorDiagNotInDebugMode:            return "JsErrorDiagNotInDebugMode";
    case JsErrorDiagNotAtBreak:                return "JsErrorDiagNotAtBreak";
    case JsErrorDiagInvalidHandle:             return "JsErrorDiagInvalidHandle";
    case JsErrorDiagObjectNotFound:            return "JsErrorDiagObjectNotFound";
    case JsErrorDiagUnableToPerformAction:     return "JsErrorDiagUnableToPerformAction";
    default:
        return "<unknown>";
        break;
    }
}

void handleJsError(JsErrorCode errCode, const char* cmd) {
  if (errCode != JsErrorScriptCompile && errCode != JsErrorScriptException) {
    Logger::error("Error %s at '%s'\n", errorCodeToString(errCode).c_str(), cmd);  
  }
  bool hasException = false;
  JsHasException(&hasException);
  if (hasException) {
    JsValueRef exception;
    JsGetAndClearException(&exception);
    printException(errCode, exception);
  }  
}

char* stringFromValue(JsValueRef valueRef) {
  JsValueRef strValue;
  JsValueType type;
  JsGetValueType(valueRef, &type);
  if (type != JsString) {
      FAIL_CHECK(JsConvertValueToString(valueRef, &strValue));
  } else {
    strValue = valueRef;
  } 
  size_t length = 0;
  FAIL_CHECK(JsCopyString(strValue, nullptr, 0, &length));
  char *cStr = nullptr;
  cStr = (char*) malloc(length + 1);
  FAIL_CHECK(JsCopyString(strValue, cStr, length + 1, nullptr));
  cStr[length] = 0;  
  return cStr;
}

JsValueRef valueFromString(std::string string) {
  JsValueRef value;
  FAIL_CHECK(JsCreateString(string.c_str(), string.length(), &value));
  return value;
}

void printException(JsErrorCode errCode, JsValueRef exception) {
  char *exceptionStr = stringFromValue(exception);
  Logger::error("Exception -> %s", exceptionStr);
  free(exceptionStr);
  Logger::error("%s", errorCodeToString(errCode).c_str());
  if (errCode == JsErrorScriptCompile) {
    int line;
    FAIL_CHECK(JsNumberToInt(JsProperty("line").get(exception), &line));

    int column;
    FAIL_CHECK(JsNumberToInt(JsProperty("column").get(exception), &column));   
    Logger::error("At: %d:%d\n", line, column);
  }
}

JsPropertyIdRef createProperty(std::string name) {
  JsPropertyIdRef propertyId = JS_INVALID_REFERENCE;
  FAIL_CHECK(JsCreatePropertyId(name.c_str(), name.length(),  &propertyId));
  return propertyId;
}

JsValueRef getProperty(JsValueRef object, std::string name) {
  JsValueRef property = JS_INVALID_REFERENCE;
  FAIL_CHECK(JsGetProperty(object, createProperty(name), &property));
  return property;
}

// JsStringWrapper

JsStringWrapper::JsStringWrapper(JsValueRef valueRef) {
  char* cStr = stringFromValue(valueRef);
  str.assign(cStr);
  free(cStr);
}

JsStringWrapper::JsStringWrapper(std::string str) : str(str) {
}

JsStringWrapper::~JsStringWrapper() {  
}

std::string JsStringWrapper::getString() {
  return str;
}

const char* JsStringWrapper::getCString() {
  return getString().c_str();
}

JsValueRef JsStringWrapper::getJsRef() {
  return valueFromString(str);
}

// JS Property

JsProperty::JsProperty(std::string name) {
  FAIL_CHECK(JsCreatePropertyId(name.c_str(), name.length(),  &propertyId));
}

JsProperty::~JsProperty() {  
}

JsValueRef JsProperty::get(JsValueRef object) {
  JsValueRef property = JS_INVALID_REFERENCE;
  FAIL_CHECK(JsGetProperty(object, propertyId, &property));
  return property;
}

void JsProperty::set(JsValueRef object, JsValueRef value) {
  FAIL_CHECK(JsSetProperty(object, propertyId, value, true));
}

void JsProperty::set(JsValueRef object, double value) {
  JsValueRef valueRef;
  FAIL_CHECK(JsDoubleToNumber(value, &valueRef));
  set(object, valueRef);
}

JsValueRef JsProperty::invoke(JsValueRef object, JsValueRef* args, size_t argCount) {  
  int fullCount = argCount + 1;
  JsValueRef fullArgs[fullCount];
  fullArgs[0] = object;
  for(int i = 0; i < argCount; i++) {
    fullArgs[i + 1] = args[i];
  }
  JsValueRef result;
  FAIL_CHECK(JsCallFunction(get(object), fullArgs, (unsigned short) fullCount, &result));
  return result;
}
