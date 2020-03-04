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

void erase_and_insert(std::multiset<int> & nums, const std::multiset<int>::iterator & it, int a) {
    nums.erase(it);
    nums.insert(a);
}

void update_pos(std::multiset<int> & nums, int a) { //удалит минимальное положительное, если а больше и добавит а
    for (auto it_num = nums.begin(); it_num != nums.end(); ++it_num) {
        if ((*it_num > 0) && (*it_num < a)) {
            erase_and_insert(nums, it_num, a);
            return;
        }
    }
}

void update_neg(std::multiset<int> & nums, int a, bool three_neg) {
    if (three_neg && (*nums.begin() < a)) {
        erase_and_insert(nums, nums.begin(), a);
    }
    else if (!three_neg) {
        auto it_num = ++nums.begin();
        if (*it_num > a) { //если минимальное по модулю отрицательное меньше |a|, то удалим его и добавим а
            erase_and_insert(nums, it_num, a); //чтобы максимизировать произведение
        }
    }
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
                update_pos(three_pos, a);
            }
            if (one_pos_two_neg.empty() || (*one_pos_two_neg.rbegin() < 0)) { //если нет положительного, добавим
                one_pos_two_neg.insert(a);
            } else {
                update_pos(one_pos_two_neg, a);
            }
        }
        else if (a < 0) {
            if (three_neg.size() < 3) three_neg.insert(a);
            else {
                update_neg(three_neg, a, true);
            }
            if (one_pos_two_neg.size() == 3) {
                update_neg(one_pos_two_neg, a, false);
            }
            else if ((one_pos_two_neg.size() <= 1) || (*one_pos_two_neg.rbegin() > 0)) {
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