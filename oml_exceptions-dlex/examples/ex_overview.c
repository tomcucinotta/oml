try
{
	// code that can raise exceptions
}
finally
{
	// code that is executed regardless of what happens within the try block. 
}
handle
	when(EEOF) {
		// EEOF exception handler
	}
	when(EIOError) {
		// EIOError exception handler
	}
end
