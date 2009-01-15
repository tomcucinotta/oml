volatile int i = 0;

try {
	i++; // i is modified inside the try-block
}
finally {
	printf("%d", i); // i is referenced inside the finally-block
}
handle
	when(EException) {
		printf("%d", i); // i is referenced inside the when-block
	}
end		
