#include <iostream>
#include <set>
#include <limits.h>

long long mul(std::multiset<int> & nums) {
    if (nums.size() < 3) return LLONG_MIN; //если недостаточно элементов в тройке, то вернем -бск
    long long ans = 1;
    for (int num : nums) {
        ans *= num;
    }
    return ans;
}

int main() {
    int N;
    std::cin >> N;
    std::multiset<int> three_pos;
    std::multiset<int> one_pos_two_neg; //произведение 1 положительного и 2 отрицательных положителльно
    std::multiset<int> three_neg; //если все числа на входе отрицательные
    bool have_zero = false; //если есть 0 во входном потоке, то имеем 0 среди произведений

    /**
     * случай 2 положителных 1 отрицательный не интересен, т.к. если N > 3, имеем 2 положительных и 1 отрицательное
     * и еще одно число, неизвестного знака, если оно положительное - то произведение 3 положительных заведомо больше
     * рассматриваемого случая, если отрицательное или равно 0 - аналогичная ситуация
     * для того чтобы не обрабатывать этот случай совсем, не перегружая код, отдельно обработаем случай N == 3
     */

    if (N == 3) {
        long long ans = 1;
        for (int i = 0; i < N; ++i) {
            int a;
            std::cin >> a;
            ans *= a;
        }
        std::cout << ans;
        return 0;
    }

    for (int i = 0; i < N; ++i) {
        int a;
        std::cin >> a;
        if (a > 0) {
            if (three_pos.size() < 3) three_pos.insert(a);
            else  {
                if (*three_pos.begin() < a) { //если минимум из положительных меньше а, то нужно запомнить а
                    three_pos.erase(three_pos.begin());
                    three_pos.insert(a);
                }
            }
            if (one_pos_two_neg.empty() || (*one_pos_two_neg.rbegin() < 0)) { //если нет положительного, добавим
                one_pos_two_neg.insert(a);
            } else {
                if (*one_pos_two_neg.rbegin() < a) { //если положительное из тройки меньше, запомним а
                    one_pos_two_neg.erase(--one_pos_two_neg.end()); //end указывает на место после max елемента
                    one_pos_two_neg.insert(a);
                }
            }
        }
        else if (a < 0) {
            if (three_neg.size() < 3) three_neg.insert(a);
            else {
                if (*three_neg.begin() < a) { //если минимум из отрицательных меньше а, запомним а
                    three_neg.erase(three_neg.begin());
                    three_neg.insert(a);
                }
            }
            if (one_pos_two_neg.size() == 3) {
                if (*one_pos_two_neg.begin() > a) { //если отрицательное меньше по модулю а, то добавим а
                    one_pos_two_neg.erase(one_pos_two_neg.begin());
                    one_pos_two_neg.insert(a);
                }
            }
            else if (one_pos_two_neg.empty() || *one_pos_two_neg.rbegin() > 0) {
                one_pos_two_neg.insert(a); //если не хватает отрицательных в тройке, то добавим
            }
        }
        else {
            have_zero = true; //нашли 0 -> получили произведение равное 0
        }
    }
    long long ans_without_0 = std::max(std::max(mul(three_pos), mul(three_neg)), mul(one_pos_two_neg));
    std::cout << (have_zero ? std::max(0ll, ans_without_0) : ans_without_0);
    return 0;
}