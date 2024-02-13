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
 * A hash table using separate chaining to handle collisions.
 */
class HashTableSeparateChaining {
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
     * The hash table using separate chaining to handle collisions.
     */
    std::vector<std::list<std::pair<int, int>>> table;

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
        if (elements / hash_groups < 3) return;

        std::vector<std::list<std::pair<int, int>>> new_table(hash_groups * 2);

        for (auto &group : table) {
            for (auto &pair : group) {
                int key = pair.first;
                int value = pair.second;

                int new_group = key % (hash_groups * 2);
                new_table[new_group].push_back({key, value});
            }
        }

        table = std::move(new_table);
        hash_groups *= 2;
    }

   public:
    /**
     * Constructs a new hash table with the given number of hash groups.
     *
     * @param hash_groups The number of hash groups to use for the hash table.
     */
    explicit HashTableSeparateChaining(int hash_groups = 10)
        : hash_groups(hash_groups), table(hash_groups) {}

    /**
     * Inserts a key-value pair into the hash table.
     *
     * @param key The key to insert into the hash table.
     * @param value The value to insert into the hash table.
     */
    void insert(int key, int value) {
        int group = key % hash_groups;
        table[group].push_back({key, value});
        elements++;
        resize();
    }

    /**
     * Removes a key-value pair from the hash table.
     *
     * @param key The key to remove from the hash table.
     */
    void remove(int key) {
        int group = key % hash_groups;

        for (auto it = table[group].begin(); it != table[group].end(); it++) {
            if (it->first == key) {
                table[group].erase(it);
                elements--;
                break;
            }
        }
    }

    /**
     * Gets the value of a key from the hash table.
     *
     * @param key The key to get the value of from the hash table.
     */
    int get(int key) {
        int group = key % hash_groups;

        for (auto it = table[group].begin(); it != table[group].end(); it++) {
            if (it->first == key) {
                return it->second;
            }
        }

        return -1;
    }
};

/**
 * Example usage of the HashTableSeparateChaining class.
 */
int main() {
    HashTableSeparateChaining hashTable(5);

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