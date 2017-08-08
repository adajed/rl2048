# rl2048
Reinforcement Learning agent for 2048 game

C++ implementation of Temporal Difference Learning for the 2048 game.<br />
Based on paper: http://www.cs.put.poznan.pl/mszubert/pub/szubert2014cig.pdf<br />

## Usage
To compile program cd to project directory and write:
```
mkdir build; cd buid
cmake ..
make
```

Then to run program:
./learn <print_every> <learning_rate> <save_every><br />
print_every - after this many games print statistics<br />
learning_rate - learning rate for algorithm<br />
save_every - after this many games save model<br />

Example usage:
```
./learn 100 0.01 1000
```

