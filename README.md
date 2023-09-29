n-ary raid prototype using Reed-Solomon coding

calling `./raid [n] [m] [all files]`, where `[all files]` consists of n×m paths will overwrite
the last m files with check data to reconstruct up to m of the n×m files in case of failure

calling `./raid [n] [m] [all files] [bad files]`, where `[bad files]` additionally consists of
up to m of the paths in `[all files]` will write the reconstructed data to those files

test multiple `[n] [m]` combinations with `./fuzz.sh`

see: <br />
http://alamos.math.arizona.edu/RTG16/ECC/raid6.pdf <br />
https://web.eecs.utk.edu/~jplank/plank/papers/CS-03-504.pdf <br />
https://web.eecs.utk.edu/~jplank/plank/papers/CS-96-332.pdf <br />
