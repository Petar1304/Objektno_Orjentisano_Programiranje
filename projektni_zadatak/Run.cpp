#include "System.h"

typedef std::string String;


System loadSystem() {
    std::cout << "Simulator mreže gradskog prevoza\nMolimo Vas, odaberite opciju:\n\t1. Učitavanje podataka o mreži gradskog prevoza\n\t0. Kraj rada.\n>>> ";
    int choice;
    std::cin >> choice;
    if (choice == 0) exit(0);
    if (choice != 1) {
        std::cout << "Uneli ste pogresan broj, pokusajte ponovo" << std::endl;
        exit(0);
    }

    String stationsFilePath;
    String busesFilePath;

    std::cout << "Molimo Vas, unesite putanju do fajla sa stajalištima:\n>>> ";
    std::cin >> stationsFilePath;

    std::cout << "Molimo Vas, unesite putanju do fajla sa linijama gradskog prevoza:\n>>> ";
    std::cin >> busesFilePath;

    return System(stationsFilePath, busesFilePath);
}


// Runs system
int main() {
    static System system = loadSystem();
    int choice;
    int id;
    String line;

    do {
        std::cout << "Mreža gradskog prevoza je uspešno učitana. Molimo Vas, odaberite opciju:\n\t1. Prikaz informacija o stajalištu\n\t2. Prikaz osnovnih informacija o liniji gradskog prevoza\n\t3. Prikaz statističkih informacija o liniji gradskog prevoza\n\t4. Pronalazak putanje između dva stajališta\n\t0. Kraj rada\n>>> ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Unesite sifru stajalista:\n>>> ";
                std::cin >> id;
                system.getStationInfo(id);
                break;

            case 2:
                std::cout << "Unesite naziv linije:\n>>> ";
                std::cin >> line;
                system.getBusInfo(line);
                break;

            case 3:
                std::cout << "Unesite naziv linije:\n>>> ";
                std::cin >> line;
                system.statistics(line);
                break;

            case 4:
                int start, end;
                String t;
                std::cout << "Unesite pocetno stajaliste:\n>>> ";
                std::cin >> start;
                std::cout << "Unesite krajnje stajaliste:\n>>> ";
                std::cin >> end;
                std::cout << "Unesite vreme u formatu [hh:mm]:\n>>> ";
                std::cin >> t;
                Time time(t);
                system.getRoute(start, end, time);
                break;
        }

    } while (choice != 0);

}