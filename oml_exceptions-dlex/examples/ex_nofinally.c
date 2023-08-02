void f()
{
	char *my_mem;

	try
	{
		my_mem = (char*) malloc(100);

		/* here some exception can be raised */

		...

		free(my_mem);
	}
	handle
		when(EEOF)
		{
			free(my_mem);

			/* EEOF handling */

			...

		}
		when(EException)
		{
			free(my_mem);

			rethrow;
		}
	end
}