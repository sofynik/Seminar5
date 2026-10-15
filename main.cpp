#include <iostream>


class Fighter {
    private:
        const char* name;
        int health;
        int maxHealth;
        int damage;
    public:
        Fighter(const char* name, int health, int damage):
        name(name), health(health), damage(damage), maxHealth(health) {}
        void TakeDamage(int amount) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
        }
    void Heal(int amount) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
        }
    void Attack (Fighter& target) const {
            target.TakeDamage(damage);
        }
    bool IsAlive() const {
            return health > 0;
        }
    void PrintStatus() const {
            std::cout << name
            << ": " << health
            << "/" << maxHealth
            << " HP, урон: " << damage
            << "\n";
        }
};
void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}
class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;
    int medkits = 1;
public:
    Game()
        : player("Игрок", 20, 8),
          enemies{{"Противник 1", 12, 3},
                  {"Противник 2", 18, 4},
                  {"Противник 3", 24, 5}} {}

    
    
    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;
        if (number < 1 || number > 3) {
            return false;
        }
        if (!enemies[number - 1].IsAlive()) {
            return false;
        }
        outTarget = &enemies[number - 1];
        return true;
    }
    bool AllEnemiesDefeated() const {
        for (int i = 0; i < 3; i++) {
            if (enemies[i].IsAlive()) {
                return false;
            }
        }
        return true;
    }
    void Run() {
        while (true) {
            std::cout << "\n--- Арена ---\n";
            PrintFighter(player);
            for (int i = 0; i < 3; i++) {
                PrintFighter(enemies[i]);
            }
            std::cout << "Аптечки: " << medkits << "\n";
            if (currentTarget != nullptr) {
                std::cout << "Текущая цель: ";
                currentTarget->PrintStatus();
            }
            else {
                std::cout << "Текущая цель не выбрана\n";
            }
            std::cout << "\n1 - выбрать цель\n";
            std::cout << "2 - атаковать\n";
            std::cout << "3 - использовать аптечку\n";
            std::cout << "0 - выйти\n";
            std::cout << "Команда: ";
            int command;
            if (!(std::cin >> command)) {
                break;
            }
            if (command == 0) {
                std::cout << "\nКонец игры.\n";
                break;
            }
            if (command == 1)
            {
                std::cout << "Выберите противника 1-3: ";
                int number;
                if (!(std::cin >> number)) {
                    break;
                }

                if (TrySelectTarget(number, currentTarget)) {
                    std::cout << "Цель выбрана.\n";
                }
                else {
                    std::cout << "Нельзя выбрать эту цель.\n";
                }
            }
            if (command == 2)
            {
                if (currentTarget == nullptr) {
                    std::cout << "Сначала выберите цель.\n";
                    continue;
                }
                player.Attack(*currentTarget);
                if (!currentTarget->IsAlive())
                {
                    PrintFighter(*currentTarget);
                    std::cout << "Противник побеждён!\n";
                    currentTarget = nullptr;
                    if (AllEnemiesDefeated())
                    {
                        std::cout << "Победа!\n";
                        break;
                    }
                    continue;
                }
                currentTarget->Attack(player);
                if (!player.IsAlive())
                {
                    PrintFighter(player);
                    std::cout << "Поражение!\n";
                    break;
                }
            }
            if (command == 3)
            {
                if (currentTarget == nullptr) {
                    std::cout << "Сначала выберите цель.\n";
                    continue;
                }
                if (medkits == 0) {
                    std::cout << "Аптечек больше нет.\n";
                    continue;
                }
                player.Heal(8);
                medkits--;
                std::cout << "Вы использовали аптечку.\n";
                currentTarget->Attack(player);
                if (!player.IsAlive())
                {
                    PrintFighter(player);
                    std::cout << "Поражение!\n";
                    break;
                }
            }
            if (command < 0 || command > 3) {
                std::cout << "Неизвестная команда.\n";
            }
        }
    }
};


int main() {
    Game game;
    game.Run();

    return 0;
}
