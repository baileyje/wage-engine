#include "jsrt/util.h"

std::string jsErrorCodeToString(JsErrorCode jsErrorCode) {
    bool hasException = false;
    JsHasException(&hasException);
    if (hasException) {
      JsValueRef exception;
      JsGetAndClearException(&exception);
      printException(exception);
    }
    switch (jsErrorCode) {
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

JsValueRef valueFromString(const char* string) {
  JsValueRef value;
  FAIL_CHECK(JsCreateString(string, strlen(string), &value));
  return value;
}

void printException(JsValueRef exception) {
  int line;
  JsValueRef lineProperty = getProperty(exception, "line");
  JsNumberToInt(lineProperty, &line);

  int column;
  JsValueRef columnProperty = getProperty(exception, "column");
  JsNumberToInt(columnProperty, &column);
 
  char *exceptionStr = stringFromValue(exception);
  printf("Exception -> %s \n", exceptionStr);
  printf("At: %d:%d\n", line, column);
  free(exceptionStr);
}

JsValueRef getProperty(JsValueRef object, std::string name) {
  JsPropertyIdRef propertyId = JS_INVALID_REFERENCE;
  JsValueRef property = JS_INVALID_REFERENCE;
  JsCreatePropertyId(name.c_str(), name.length(),  &propertyId);
  JsGetProperty(object, propertyId, &property);
}

bool createNamedFunction(const char* nameString, JsNativeFunction callback, JsValueRef* functionVar) {
  FAIL_CHECK(JsCreateNamedFunction(valueFromString(nameString), callback, nullptr, functionVar));
  return true;
}

bool installObjectsOnObject(JsValueRef object, const char* name, JsNativeFunction nativeFunction) {
  JsValueRef propertyValueRef;
  JsPropertyIdRef propertyId;
  FAIL_CHECK(JsCreatePropertyId(name, strlen(name), &propertyId));
  if (!createNamedFunction(name, nativeFunction, &propertyValueRef)) {
      return false;
  }
  FAIL_CHECK(JsSetProperty(object, propertyId, propertyValueRef, true));
  return true;
}

JSString::JSString(JsValueRef ref) : valueRef(ref)  {  
}

JSString::JSString(char* cSrt)  {
  str.assign(cSrt);
}

JSString::JSString(std::string str) : str(str) {
}

JSString::~JSString() {  
}

std::string JSString::getString() {
  if (str.empty()) {
    char* cStr = stringFromValue(valueRef);
    str.assign(cStr);
    free(cStr);    
  }
  return str;
}

const char* JSString::getCString() {
  return getString().c_str();
}

JsValueRef JSString::getJSRef() {
  return valueRef;
}
