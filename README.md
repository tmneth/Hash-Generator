# Hash function

My attempt to implement hash function somewhat similar to sha-256.

## Project Setup

Clone the project

```
git clone https://github.com/tmneth/hash-generator.git
```

Initialise and update submodules [to use existing hash functions](https://github.com/stbrumme/hash-library)

```
git submodule init
git submodule update
```

Compile the files

```
g++ src/*.cpp lib/hash-library/{sha256,md5,sha1,keccak}.cpp -o 'main' && ./main
```

## User Interface

Once compiled, user will be presented with the following options:

```c++
1. Hash a string
2. Hash a file
3. I/O test
4. Measure time complexity
5. Test on collisions
6. Test on Avalanche effect
7. Compare hashing time to sha256, md5, sha1, keccak
```

*Note, for a file to be hashed, it has to be passed as a `Command Line Argument`

```
./main some_text_file.txt
```

## Hash Function

To understand how the algorithm works check out [step-by-step hash of “hello world”](example.md)

## Analysis

### Part 1 - Input & Output

For the test purposes 7 different text files were created.
Every single file was hashed twice, hexes were compared.

```
Hashing twice file: letter.txt; Word size: 64
Hash (1): c56ebd88c9afd0981201c4de665372aad244bbf8861f54743fecd1d484caff2c
Hash (2): c56ebd88c9afd0981201c4de665372aad244bbf8861f54743fecd1d484caff2c
Hashes are: identical

Hashing twice file: char.txt; Word size: 64
Hash (1): 509edaaa536b19e8c44a4bd484f27358f05f4c70fb51ce900a8f11388be4e8d4
Hash (2): 509edaaa536b19e8c44a4bd484f27358f05f4c70fb51ce900a8f11388be4e8d4
Hashes are: identical

Hashing twice file: rand_1000_1.txt; Word size: 64
Hash (1): be5414a42b887e3026f76784ab760408f4515fd4b14859b0774f0d588fb08098
Hash (2): be5414a42b887e3026f76784ab760408f4515fd4b14859b0774f0d588fb08098
Hashes are: identical

Hashing twice file: rand_1000_2.txt; Word size: 64
Hash (1): 6dae59dcf00227f01114ee783aa083e0c02ba9b0ae91d82033e42608e1d33678
Hash (2): 6dae59dcf00227f01114ee783aa083e0c02ba9b0ae91d82033e42608e1d33678
Hashes are: identical

Hashing twice file: sim_1500_1.txt; Word size: 64
Hash (1): 62610d2829307e5c2f051b700ad57dd85e5e2fe05297545434d52fb4e25b37d0
Hash (2): 62610d2829307e5c2f051b700ad57dd85e5e2fe05297545434d52fb4e25b37d0
Hashes are: identical

Hashing twice file: sim_1500_2.txt; Word size: 64
Hash (1): 5e31f454f557c94c268adbe892b049f0d20e2e98aa9892e44f348708e1292800
Hash (2): 5e31f454f557c94c268adbe892b049f0d20e2e98aa9892e44f348708e1292800
Hashes are: identical

Hashing twice file: empty.txt; Word size: 64
Hash (1): 0152fb74025769d049710ea4f8f46cbcb539d6b88fdd309866860174ecc54ea0
Hash (2): 0152fb74025769d049710ea4f8f46cbcb539d6b88fdd309866860174ecc54ea0
Hashes are: identical

Conclusion: hashing algorithm is deterministic.
```

### Part 2 - Time Complexity

To test execution time, it was measured how much time it takes for the function to hash 1,2,4,8...512 lines from file
konstitucija.txt - overall time grows linearly. (100 hashing rounds were performed & the
average time was calculated).

Time complexity of the hash function is O(n).

| Lines | 1          | 2          | 4          | 8          | 16         | 32         | 64          | 128        | 256         | 512       |
|-------|------------|------------|------------|------------|------------|------------|-------------|------------|-------------|-----------|
| Time  | 0.000003s. | 0.000006s. | 0.000011s. | 0.000023s. | 0.000046s. | 0.000092s. | 0.0000183s. | 0.000366s. | 0.0007730s. | 0.001455. |

### Part 3 - Collision Test

For this test pairs of randomly generated lines were hashed and compared.

No collisions were found.

### Part 4 - Avalanche Effect Test

Overall, MYSHA performance results are nearly identical to those of SHA256.

However, MYSHA beats all the function by the average and biggest bitwise difference of 67.57% and 77.34% respectively.

MYSHA: 
```
Avg difference (hex): 91.48%
Avg difference (bit): 67.57%
Min difference (hex): 73.44%
Min difference (bit): 57.03%
Max difference (hex): 100.00%
Max difference (bit): 77.34%
```
SHA256:
```
Avg difference (hex): 93.74%
Avg difference (bit): 66.99%
Min difference (hex): 76.56%
Min difference (bit): 57.23%
Max difference (hex): 100.00%
Max difference (bit): 76.37%
```
MD5: 
```
Avg difference (hex): 93.73%
Avg difference (bit): 33.49%
Min difference (hex): 68.75%
Min difference (bit): 27.15%
Max difference (hex): 100.00%
Max difference (bit): 40.23%
```
SHA1:
```
Avg difference (hex): 93.75%
Avg difference (bit): 41.87%
Min difference (hex): 70.00%
Min difference (bit): 34.96%
Max difference (hex): 100.00%
Max difference (bit): 49.22%
```
KECCAK:
```
Avg difference (hex): 93.74%
Avg difference (bit): 66.99%
Min difference (hex): 76.56%
Min difference (bit): 56.64%
Max difference (hex): 100.00%
Max difference (bit): 76.17%
```

### Part 5 - Comparison With Other Functions

For this test, functions had to hash every single line of konstitucija.txt. (100 hashing rounds were performed & the
average time was calculated). -O3 optimization was used for this test.

Compared to existing hash functions, MYSHA was the slowest.

```
MYSHA average hashing time: 0.00225199s.
SHA256 average hashing time: 0.00142127s.
MD5 average hashing time: 0.00114694s.
SHA1 average hashing time: 0.00141966s.
KECCAK average hashing time: 0.00158276s.
```