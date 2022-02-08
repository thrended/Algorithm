# project-3-cuckoo-hashing

Group members:

Wayne Lin   waylin@csu.fullerton.edu

In this project I implemented a Cuckoo Hashing algorithm to resolve hash table collisions. Two hash tables with different functions stored entries read from the text file. I had to concatenate the null character onto the end of each string because s[len-1] = '/0' was deleting the last letter of the string, leading to incorrect output values. I implemented a polynomial coding hash function to generate the second hash table, using Horner's method to slightly improve the runtime and applying the prime modulus to every step in order to prevent integer overflow. Finally, I designed code to evict strings and switch between tables as seen in interactive examples in class.
