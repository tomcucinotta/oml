void f()
{
	char *my_mem;

	try 
	{
		my_mem = (char*) malloc(100);

		/* here some exception can be raised */		

		...

	}
	finally 
	{
		free(my_mem);
	}
	handle
		when(EEOF)
		{

			/* EEOF handling */

			...

		}		
	end
}
