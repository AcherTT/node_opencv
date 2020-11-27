#include "image.h"

napi_value FrameCorrection(napi_env env, napi_callback_info args)
{
	argumentsDir *MyArgs = GetArgumentsDir(env, args);
	int result = GetContoursPic(MyArgs->imageSrc.c_str(), MyArgs->outPutSrc.c_str());
	returnValue(result);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, "FrameCorrection", NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value CharacterCorrection(napi_env env, napi_callback_info args)
{
	argumentsDir *MyArgs = GetArgumentsDir(env, args);
	int result = ImageRecify(MyArgs->imageSrc.c_str(), MyArgs->outPutSrc.c_str());
	returnValue(result);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, "CharacterCorrection", NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value DistortionCorrection(napi_env env, napi_callback_info args)
{
	argumentsDir *MyArgs = GetArgumentsDir(env, args);
	int result = CornerCheck(MyArgs->imageSrc.c_str(), MyArgs->outPutSrc.c_str());
	returnValue(result);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, "DistortionCorrection", NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value FrameCorrectionBase64(napi_env env, napi_callback_info args)
{
	argumentsBase64 *MyArgs = GetArgumentsBase64(env, args);
	string result;
	cout << MyArgs->type << endl;
	int code = GetContoursPic(MyArgs->base64Data, result, MyArgs->type);
	cout<<code<<endl;
	returnValue(code);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value CharacterCorrectionBase64(napi_env env, napi_callback_info args)
{
	argumentsBase64 *MyArgs = GetArgumentsBase64(env, args);
	string result;
	int code = ImageRecify(MyArgs->base64Data, result, MyArgs->type);
	returnValue(code);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value DistortionCorrectionBase64(napi_env env, napi_callback_info args)
{
	argumentsBase64 *MyArgs = GetArgumentsBase64(env, args);
	string result;
	int code = CornerCheck(MyArgs->base64Data, result, MyArgs->type);
	returnValue(code);
	napi_value greeting;
	napi_status status3 = napi_create_string_utf8(env, result.c_str(), NAPI_AUTO_LENGTH, &greeting);
	delete MyArgs;
	return greeting;
}

napi_value init(napi_env env, napi_value exports)
{
	napi_value fn1;
	napi_value fn2;
	napi_value fn3;
	napi_value fn4;
	napi_value fn5;
	napi_value fn6;
	napi_status status1 = napi_create_function(env, nullptr, 0, FrameCorrection, nullptr, &fn1);
	napi_status status2 = napi_create_function(env, nullptr, 0, CharacterCorrection, nullptr, &fn2);
	napi_status status3 = napi_create_function(env, nullptr, 0, DistortionCorrection, nullptr, &fn3);
	napi_status status4 = napi_create_function(env, nullptr, 0, FrameCorrectionBase64, nullptr, &fn4);
	napi_status status5 = napi_create_function(env, nullptr, 0, CharacterCorrectionBase64, nullptr, &fn5);
	napi_status status6 = napi_create_function(env, nullptr, 0, DistortionCorrectionBase64, nullptr, &fn6);
	if (status1 != napi_ok || status2 != napi_ok || status3 != napi_ok || status4 != napi_ok || status5 != napi_ok || status6 != napi_ok)
	{
		return nullptr;
	}

	napi_status status7 = napi_set_named_property(env, exports, "FrameCorrection", fn1);
	napi_status status8 = napi_set_named_property(env, exports, "CharacterCorrection", fn2);
	napi_status status9 = napi_set_named_property(env, exports, "DistortionCorrection", fn3);
	napi_status status10 = napi_set_named_property(env, exports, "FrameCorrectionBase64", fn4);
	napi_status status11 = napi_set_named_property(env, exports, "CharacterCorrectionBase64", fn5);
	napi_status status12 = napi_set_named_property(env, exports, "DistortionCorrectionBase64", fn6);
	if (status7 != napi_ok || status8 != napi_ok || status9 != napi_ok || status10 != napi_ok || status11 != napi_ok || status12 != napi_ok)
	{
		return nullptr;
	}

	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)