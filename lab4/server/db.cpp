#include <iostream>
#include <vector>
#include <string>

// Struktura reprezentująca postać
struct Character {
    int ID;
    std::string surname;
};

// Baza danych postaci
std::vector<Character> characters;

// Funkcja do dodawania postaci do bazy danych
void addCharacter(int ID, const std::string& surname) {
    Character newCharacter = {ID, surname};
    characters.push_back(newCharacter);
}

// Funkcja do wyświetlania bazy danych postaci
void displayCharacters() {
    std::cout << "Baza danych postaci:" << std::endl;
    for (const auto& character : characters) {
        std::cout << "ID: " << character.ID << ", Nazwisko: " << character.surname << std::endl;
    }
}

// Funkcja do pobierania postaci na podstawie ID
char* getCharacterByID(int ID) {
    for (const auto& character : characters) {
        if (character.ID == ID) {
            return (char*) character.surname.c_str();
        }
    }
    // Zwracamy pustą postać, jeśli nie znaleziono
    return (char*) "not found";
}

void prepareDatabase() {
    addCharacter(1, "Kowalski");
    addCharacter(2, "Nowak");
    addCharacter(3, "Kowalczyk");
    addCharacter(4, "Kamiński");
    addCharacter(5, "Lewandowski");
}