#include "ModuleParson.h"
#include "Module.h"
#include "Application.h"

/*
static int malloc_count;
static void* counted_malloc(size_t size);
static void counted_free(void *ptr);
*/

/*
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
*/

ParsonJSON::ParsonJSON()
{
	//json_set_allocation_functions(counted_malloc, counted_free);
	root_value = json_value_init_object();
	root_object = json_value_get_object(root_value);
	file_name = "conf.json";
}

ParsonJSON::~ParsonJSON()
{
	json_value_free(root_value);
}

bool ParsonJSON::Init()
{
	root_value = json_parse_file(file_name.c_str());
	if (root_value == NULL)
	{
		root_value = json_value_init_object();
		json_serialize_to_file(root_value, file_name.c_str());
	}
	else
		root_object = json_value_get_object(root_value);
	
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
		if (json_object_get_object(root_object, (*item)->name.c_str()) == NULL)
			json_object_set_value(root_object, (*item)->name.c_str(), json_value_init_object());
	return true;
}

bool ParsonJSON::Save() const
{
	JSON_Object* conf = nullptr;
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
	{
		conf = json_object_get_object(root_object, (*item)->name.c_str());
		(*item)->SaveConf(conf);
	}
	json_serialize_to_string_pretty(root_value);
	json_serialize_to_file(root_value, file_name.c_str());
	return true;
}

bool ParsonJSON::Load()
{
	JSON_Object* conf = nullptr;
	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
	{
		conf = json_object_get_object(root_object, (*item)->name.c_str());
		(*item)->LoadConf(conf);
	}
	return true;
}