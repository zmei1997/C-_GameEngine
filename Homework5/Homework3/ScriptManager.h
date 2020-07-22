#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "duktape.h"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <iostream>

#include <dukglue/dukglue.h>
#include "scriptObject.h"
#include "GameObject.h"
using namespace std;

static void load_script_from_file(duk_context* ctx, const char* filename)
{																		  
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	duk_push_lstring(ctx, buffer.str().c_str(), (duk_size_t)(buffer.str().length()));
}

static duk_ret_t native_print(duk_context* ctx)
{
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

class ScriptManager
{
private:
	int r = 0;
	int g = 0;
	int b = 0;
	float new_length = 0;
	float new_width = 0;
public:
	ScriptManager();
	int scriptChangeColor();
	int getR_fromScript();
	int getG_fromScript();
	int getB_fromScript();
	float getLength_fromScript();
	float getWidth_fromScript();
};

