#include <iostream>

class Fighter
{
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;
public:
    Fighter(const char* name, int health, int damage)
        : name(name), health(health), maxHealth(health), damage(damage){}
    void TakeDamage(int amount)
    {
        health -= amount;
        if (health < 0)
        {
            health = 0;
        }
    }
    void Attack(Fighter& target) const
    {
        target.TakeDamage(damage);
    }
    bool IsAlive() const
    {
        return health > 0;
    }
    void PrintStatus() const
    {
        std::cout << name << ": "
                  << health << "/" << maxHealth
                  << " здоровья, урон: " << damage << '\n';
    }
};

void PrintFighter(const Fighter& fighter)
{
    fighter.PrintStatus();
}
class Game
{
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget;
    bool TrySelectTarget(int number, Fighter*& outTarget)
    {
        outTarget = nullptr;
        if (number < 1 || number > 3)
        {
            return false;
        }
        if (!enemies[number - 1].IsAlive())
        {
            return false;
        }
        outTarget = &enemies[number - 1];
        return true;
    }
    bool AllEnemiesDefeated() const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (enemies[i].IsAlive())
            {
                return false;
            }
        }
        return true;
    }

public:
    Game()
        : player("Игрок", 30, 8),
          enemies{
              {"Противник 1", 12, 3},
              {"Противник 2", 18, 4},
              {"Противник 3", 24, 5}
          },
          currentTarget(nullptr){}
    void Run()
    {
        while (true)
        {
            std::cout << "\n--- Арена ---\n";
            std::cout << "Игрок:\n";
            PrintFighter(player);
            std::cout << "\nПротивники:\n";
            for (int i = 0; i < 3; ++i)
            {
                std::cout << i + 1 << ". ";
                PrintFighter(enemies[i]);
            }
            std::cout << "\nТекущая цель: ";
            if (currentTarget != nullptr)
            {
                currentTarget->PrintStatus();
            }
            else
            {
                std::cout << "нет\n";
            }
            std::cout << "\n1 - выбрать цель\n";
            std::cout << "2 - атаковать\n";
            std::cout << "0 - выйти\n";
            std::cout << "Команда: ";
            int command;
            if (!(std::cin >> command))
            {
                break;
            }
            if (command == 0)
            {
                break;
            }
            if (command == 1)
            {
                int number;
                std::cout << "Выберите противника (1-3): ";
                if (!(std::cin >> number))
                {
                    break;
                }
                if (TrySelectTarget(number, currentTarget))
                {
                    std::cout << "Цель выбрана.\n";
                }
                else
                {
                    std::cout << "Нельзя выбрать эту цель.\n";
                }
            }
            else if (command == 2)
            {
                if (currentTarget == nullptr)
                {
                    std::cout << "Цель не выбрана.\n";
                    continue;
                }
                player.Attack(*currentTarget);
                if (!currentTarget->IsAlive())
                {
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
                    std::cout << "Поражение!\n";
                    break;
                }
            }
        }
    }
};

int main()
{
    Game game;
    game.Run();
    return 0;
}