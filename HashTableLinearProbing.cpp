//
// Copyright Caiden Sanders - All Rights Reserved
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
//
// Written by Caiden Sanders <work.caidensanders@gmail.com>, February 12, 2024.
//

#include <math.h>

#include <iostream>
#include <list>
#include <vector>

/**
 * A hash table using linear probing to handle collisions.
 */
class HashTableLinearProbing {
   private:
    /**
     * The number of hash groups to use for the hash table.
     */
    int hash_groups = 10;

    /**
     * The number of elements in the hash table.
     */
    int elements = 0;

    /**
     * The hash table using linear probing to handle collisions.
     */
    std::vector<std::pair<int, int>> table;

    /**
     * Hash function using Horner's method to compute the hash value of a given
     * string.
     *
     * @param s The string to compute the hash value of.
     * @param base The base to use for the hash function.
     * @param mod The modulo to use for the hash function.
     */
    int horner_hash(const std::string &s, int base = 31,
                    int mod = pow(10, 9) + 9) {
        int hash_value = 0;

        for (char c : s) {
            hash_value = (1LL * hash_value * base + c) % mod;
        }

        return hash_value;
    }

    /**
     * Resizes the hash table when the number of elements exceeds the number of
     * hash groups.
     *
     * This is done to ensure that the hash table is always efficient and
     * doesn't have too many elements in a single hash group.
     *
     * The hash table is resized by doubling the number of hash groups.
     */
    void resize() {
        hash_groups *= 2;

        std::vector<std::pair<int, int>> new_table(hash_groups);

        for (const auto &p : table) {
            int index = p.first % hash_groups;

            while (new_table[index].first != 0) {
                index = (index + 1) % hash_groups;
            }

            new_table[index] = p;
        }

        table = new_table;
    }

   public:
    /**
     * Constructs a new hash table with the given number of hash groups.
     *
     * @param hash_groups The number of hash groups to use for the hash table.
     */
    explicit HashTableLinearProbing(int hash_groups = 10)
        : hash_groups(hash_groups), table(hash_groups) {}

    /**
     * Inserts a key-value pair into the hash table.
     *
     * @param key The key to insert into the hash table.
     * @param value The value to insert into the hash table.
     */
    void insert(int key, int value) {
        if (elements >= hash_groups * 0.75) {
            resize();
        }

        int index = key % hash_groups;

        while (table[index].first != 0 && table[index].first != key) {
            index = (index + 1) % hash_groups;
        }

        if (table[index].first != key) {
            elements++;  // Increment only for new keys.
        }

        table[index] = {key, value};
    };

    /**
     * Removes a key-value pair from the hash table.
     *
     * @param key The key to remove from the hash table.
     */
    void remove(int key) {
        int index = key % hash_groups;

        while (table[index].first != key) {
            index = (index + 1) % hash_groups;
        }

        table[index] = {0, 0};
        elements--;
    }

    /**
     * Gets the value of a key from the hash table.
     *
     * @param key The key to get the value of from the hash table.
     */
    int get(int key) {
        int index = key % hash_groups;
        int start_index =
            index;  // Remember the starting index to detect a full loop.

        // Loop until we find an empty slot or have checked the entire table.
        while (table[index].first != 0) {
            if (table[index].first == key) {
                return table[index]
                    .second;  // Return the value if the key is found.
            }
            index = (index + 1) % hash_groups;
            if (index == start_index)
                break;  // Break if we've looped back to the start.
        }

        return -1;  // Indicate that the key was not found.
    }
};

/**
 * Example usage of the HashTableLinearProbing class.
 */
int main() {
    HashTableLinearProbing hashTable(5);

    hashTable.insert(1, 100);
    hashTable.insert(2, 200);
    hashTable.insert(3, 300);

    std::cout << "Value for key 2: " << hashTable.get(2) << std::endl;

    hashTable.remove(2);

    std::cout << "Value for key 2 after removal: " << hashTable.get(2)
              << std::endl;

    for (int i = 4; i <= 20; i++) {
        hashTable.insert(i, i * 100);
    }

    std::cout << "Value for key 20: " << hashTable.get(20) << std::endl;

    return 0;
}