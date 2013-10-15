#include <stdio.h>
#include <list>

typedef std::list<int> Partition;

long total = 0;
const int n_min = 1;
const int n_max = 2;
const int N = 26;

void print_list(const Partition& P){
    printf("(");
    Partition::const_iterator it;
    for(it=P.begin(); it!=P.end(); ++it){
        printf("%i,", *it);
    }
    printf(")\n");
}

bool is_unique(const Partition& P){
    Partition::const_iterator it_forward = P.begin();
    Partition::const_iterator it_backward = --P.end();
    while (true){
        if (*it_forward<*it_backward)
            return false;
        if (*it_forward>*it_backward)
            return true;
        if (++it_forward==it_backward || it_forward==--it_backward)
            return true;
    }

}

void step(Partition& P, int sum){
    int new_sum;
    for(int i=n_min; i<=n_max; ++i){
        new_sum = sum+i;
        if (new_sum==N){
            P.push_back(i);
            //print_list(P);
            if (is_unique(P))
                total += 1;
            P.pop_back();
            P.pop_back();
            return;
        }
        else if (new_sum>N){
            P.pop_back();
            return;
        }
        P.push_back(i);
        step(P, new_sum);
    }
    if (P.size()>0)
        P.pop_back();
}

int main(int argc, const char* argv[]){
    Partition P;
    step(P,0);
    printf("%ld", total);
}
