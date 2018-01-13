#include <iostream>
#include <ctime>
#include <cstdlib>
    
#define RAND_MIN_VAL 35
#define RAND_MAX_VAL 122

static unsigned seed = 0;

class Rain {

public:
    Rain() {
        
        head = gen_random();
    }

    friend std::ostream& operator << (std::ostream& o, Rain const& r) {

        if(r.enabled) {

            return o << r.get_head();
        }
        else {

            return o << ' ';
        }
    }

    char gen_random() const {

        srand(seed++);
        return (char)(RAND_MIN_VAL + (std::rand() % static_cast<int>(RAND_MAX_VAL - RAND_MIN_VAL + 1)));
    }

    char get_head() const {

        return head;
    }

    bool is_enabled() const {

        return enabled;
    }

    void enable() {

        enabled = true;
    }

    void disable() {

        enabled = false;
    }

private:
    bool enabled;
    char head;
};