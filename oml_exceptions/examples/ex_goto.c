goto label1; // error

try {

	label1:

		goto label2; //ok

		goto label3; //error

		...

	label2:

		...
}
finally {

		...

		goto label4:

		...

	label4:

		...
}
handle
end

...

label3:

...
