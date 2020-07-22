#include "ScriptManager.h"
#include <regex>

ScriptManager::ScriptManager() {}

int ScriptManager::scriptChangeColor() {
	duk_context* ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		exit(1);
	}

	// Register objects and member functions inside our context
	dukglue_register_constructor<scriptObject>(ctx, "scriptObject");
	dukglue_register_method(ctx, &scriptObject::GetR, "get_r");
	dukglue_register_method(ctx, &scriptObject::GetG, "get_g");
	dukglue_register_method(ctx, &scriptObject::GetB, "get_b");
	dukglue_register_method(ctx, &scriptObject::GetLenght, "get_length");
	dukglue_register_method(ctx, &scriptObject::GetWidth, "get_width");
	dukglue_register_method(ctx, &scriptObject::SetR, "set_r");
	dukglue_register_method(ctx, &scriptObject::SetG, "set_g");
	dukglue_register_method(ctx, &scriptObject::SetB, "set_b");
	dukglue_register_method(ctx, &scriptObject::SetLength, "set_length");
	dukglue_register_method(ctx, &scriptObject::SetWidth, "set_width");

	// Can use the standard duktape API to register c_functions if necessary
	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");

	// Load script from file, evaluate script
	load_script_from_file(ctx, "scriptingForSpaceInv.js");
	if (duk_peval(ctx) != 0) {
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		duk_destroy_heap(ctx);
		return 1;
	}
	duk_pop(ctx); // Ignore return, clear stack

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "enableScript");

	/* This demonstrates passing objects declared in C++ to scripts */
	scriptObject newScriptObject;
	dukglue_push(ctx, &newScriptObject);

	if (duk_pcall(ctx, 1) != 0)
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
	else
		printf("%s\n", duk_safe_to_string(ctx, -1));

	// receive a string and then split them into integer
	string s = duk_safe_to_string(ctx, -1);
	//cout << s << endl;
	std::regex regex(",");
	std::vector<std::string> out(
		std::sregex_token_iterator(s.begin(), s.end(), regex, -1),
		std::sregex_token_iterator()
	);
	vector<int> buffer;
	for (auto& s : out) {
		//std::cout << s << '\n';
		buffer.push_back(stoi(s)); // convert all strings to integers, and stored in a buffer
	}

	//cout << buffer[0] << endl;
	r = buffer[0];
	//cout << buffer[1] << endl;
	g = buffer[1];
	//cout << buffer[2] << endl;
	b = buffer[2];

	new_length = buffer[3];
	new_width = buffer[4];

	duk_pop(ctx);

	duk_destroy_heap(ctx);

	return 0;
}

int ScriptManager::getR_fromScript() {
	return r;
}
int ScriptManager::getG_fromScript() {
	return g;
}
int ScriptManager::getB_fromScript() {
	return b;
}
float ScriptManager::getLength_fromScript() {
	return new_length;
}
float ScriptManager::getWidth_fromScript() {
	return new_width;
}