#include "o2/stdafx.h"

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "o2/Scripts/ScriptValue.h"

namespace o2
{
	ScriptValueBase::~ScriptValueBase()
	{
		jerry_release_value(jvalue);
	}

	void ScriptValueBase::AcquireValue(jerry_value_t v)
	{
		jerry_release_value(jvalue);
		jvalue = jerry_acquire_value(v);
	}

	void ScriptValueBase::Accept(jerry_value_t v)
	{
		jerry_release_value(jvalue);
		jvalue = v;
	}

	ScriptValue::ScriptValue()
	{
		jvalue = jerry_create_undefined();
	}

	ScriptValue::ScriptValue(const ScriptValue& other)
	{
		jvalue = jerry_acquire_value(other.jvalue);
	}

	ScriptValue ScriptValue::operator[](const ScriptValue& name)
	{
		return GetProperty(name);
	}

	ScriptValue ScriptValue::operator[](int idx)
	{
		ScriptValue res;
		res.jvalue = jerry_get_property_by_index(jvalue, idx);
		return res;
	}

	bool ScriptValue::operator!=(const ScriptValue& other) const
	{
		return !jerry_binary_operation(JERRY_BIN_OP_EQUAL, jvalue, other.jvalue);
	}

	bool ScriptValue::operator==(const ScriptValue& other) const
	{
		return jerry_binary_operation(JERRY_BIN_OP_EQUAL, jvalue, other.jvalue);
	}

	ScriptValue& ScriptValue::operator=(const ScriptValue& other)
	{
		jerry_release_value(jvalue);
		jvalue = jerry_acquire_value(other.jvalue);
		return *this;
	}

	ScriptValue::ValueType ScriptValue::GetValueType() const
	{
		if (jerry_value_is_array(jvalue))
			return ValueType::Array;

		return (ValueType)jerry_value_get_type(jvalue);
	}

	int ScriptValue::GetLength() const
	{
		if (!jerry_value_is_array(jvalue))
			return 0;

		return jerry_get_array_length(jvalue);
	}
	
	String ScriptValue::GetError() const
	{
		if (GetValueType() != ValueType::Error)
			return String();

		static Map<jerry_error_t, const char*> errorsMap = {
			{ JERRY_ERROR_COMMON, "JERRY_ERROR_COMMON" },
			{ JERRY_ERROR_EVAL, "JERRY_ERROR_EVAL" },
			{ JERRY_ERROR_RANGE, "JERRY_ERROR_RANGE" },
			{ JERRY_ERROR_REFERENCE, "JERRY_ERROR_REFERENCE" },
			{ JERRY_ERROR_SYNTAX, "JERRY_ERROR_SYNTAX" },
			{ JERRY_ERROR_TYPE, "JERRY_ERROR_TYPE" },
			{ JERRY_ERROR_URI, "JERRY_ERROR_URI" }
		};

		return errorsMap[jerry_get_error_type(jvalue)];
	}

	void ScriptValue::ForEachProperties(const Function<bool(const ScriptValue& name, const ScriptValue& value)>& func) const
	{
		struct Helper
		{
			static bool IterateFunc(const jerry_value_t property_name, const jerry_value_t property_value, void* user_data_p)
			{
				auto func = (Function<bool(const ScriptValue&, const ScriptValue&)>*)user_data_p;
				ScriptValue name, value;
				name.jvalue = property_name;
				value.jvalue = property_value;
				return (*func)(name, value);
			}
		};

		if (GetValueType() != ValueType::Object)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_object(jvalue);
			jerry_release_value(prev);
		}

		jerry_foreach_object_property(jvalue, &Helper::IterateFunc, (void*)&func);
	}

	ScriptValue ScriptValue::GetProperty(const ScriptValue& name) const
	{
		if (GetValueType() != ValueType::Object)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_object(jvalue);
			jerry_release_value(prev);
		}

		ScriptValue res;
		jerry_release_value(res.jvalue);
		res.jvalue = jerry_get_property(jvalue, name.jvalue);
		return res;
	}

	void ScriptValue::SetProperty(const ScriptValue& name, const ScriptValue& value)
	{
		if (GetValueType() != ValueType::Object)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_object(jvalue);
			jerry_release_value(prev);
		}

		jerry_set_property(jvalue, name.jvalue, value.jvalue);
	}

	void ScriptValue::SetElement(const ScriptValue& value, int idx)
	{
		if (GetValueType() != ValueType::Array)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_object(jvalue);
			jerry_release_value(prev);
		}

		jerry_set_property(jvalue, ScriptValue(idx), value.jvalue);
	}

	void ScriptValue::AddElement(const ScriptValue& value)
	{
		SetElement(value, GetLength());
	}

	bool ScriptValue::ToBool() const
	{
		return jerry_value_to_boolean(jvalue);
	}

	float ScriptValue::ToNumber() const
	{
		if (GetValueType() != ValueType::Number)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_number(jvalue);
			jerry_release_value(prev);
		}

		return (float)jerry_get_number_value(jvalue);
	}

	String ScriptValue::ToString() const
	{
		if (GetValueType() != ValueType::Number)
		{
			auto prev = jvalue;
			jvalue = jerry_value_to_string(jvalue);
			jerry_release_value(prev);
		}

		String res;
		res.resize(jerry_get_string_length(jvalue) + 1);
		jerry_string_to_char_buffer(jvalue, (jerry_char_t*)res.Data(), res.Capacity());
		return std::move(res);
	}

	ScriptValue ScriptValue::InvokeRaw(const Vector<ScriptValue>& args) const
	{
		return InvokeRaw(ScriptValue(), args);
	}

	ScriptValue ScriptValue::InvokeRaw(const ScriptValue& thisValue, const Vector<ScriptValue>& args) const
	{
		if (GetValueType() == ValueType::Function)
		{
			const int maxParameters = 16;
			jerry_value_t valuesBuf[maxParameters];
			for (int i = 0; i < args.Count() && i < maxParameters; i++)
				valuesBuf[i] = args[i].jvalue;

			auto res = jerry_call_function(jvalue, ScriptValue().jvalue, valuesBuf, args.Count());

			ScriptValue resValue;
			jerry_release_value(resValue.jvalue);
			resValue.jvalue = res;

			return resValue;
		}

		return {};
	}

	ScriptValueBase::DataContainerDeleter& ScriptValueBase::GetDataDeleter()
	{
		static DataContainerDeleter deleter;
		return deleter;
	}

	ScriptValueBase::DataContainerDeleter::DataContainerDeleter()
	{
		info.free_cb = &Free;
	}

	void ScriptValueBase::DataContainerDeleter::Free(void* ptr)
	{
		delete (IDataContainer*)ptr;
	}

	jerry_value_t ScriptValueBase::CallFunction(const jerry_value_t function_obj,
												const jerry_value_t this_val,
												const jerry_value_t args_p[], const jerry_length_t args_count)
	{
		void* ptr = nullptr;
		jerry_get_object_native_pointer(function_obj, &ptr, &GetDataDeleter().info);

		IFunctionContainer* container = dynamic_cast<IFunctionContainer*>((IDataContainer*)ptr);
		return container->Invoke(this_val, (jerry_value_t*)args_p, args_count);
	}
}

#endif