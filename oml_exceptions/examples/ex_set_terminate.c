PFV old_handler;

void My_Terminate()
{
	puts("Program aborted: exception not handled");
	(*old_handler)();
}

main()
{
	...

	old_handler = set_terminate( &My_Terminate );
	...

}
 