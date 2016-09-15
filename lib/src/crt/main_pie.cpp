//extern void __cdecl pie_initialize_statics();

extern int main(int argc, char ** argv);

int main_pie_init(int argc, char ** argv)
{
	//pie_initialize_statics();

	return main(argc, argv);
}