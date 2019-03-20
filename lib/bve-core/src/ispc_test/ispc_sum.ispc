export uniform unsigned int64 sum(uniform unsigned int64 array[], uniform unsigned int64 size) {
	varying size_t accumulator = 0;

	foreach (i = 0 ... size) {
		accumulator += array[i];
	}

	return reduce_add(accumulator);
}
