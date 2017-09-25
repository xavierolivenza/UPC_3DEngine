#include "ModuleParson.h"

static int malloc_count;
static void* counted_malloc(size_t size);
static void counted_free(void *ptr);

ModuleParson::ModuleParson(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "parson";
	json_set_allocation_functions(counted_malloc, counted_free);
	root_value = json_value_init_object();
	root_object = json_value_get_object(root_value);
	file_name = "conf.json";
}

ModuleParson::~ModuleParson()
{

}

bool ModuleParson::Init()
{
	return true;
}

bool ModuleParson::CleanUp()
{
	return true;
}

bool ModuleParson::Save()
{
	return true;
}

bool ModuleParson::Load()
{
	return true;
}

static void* counted_malloc(size_t size) {
	void* res = malloc(size);
	if (res != nullptr)
		malloc_count++;
	return res;
}

static void counted_free(void* ptr) {
	if (ptr != nullptr)
		malloc_count--;
	free(ptr);
}