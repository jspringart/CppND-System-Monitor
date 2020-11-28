#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long prevActive{0};
    long active{0};
    long prevIdle{0};
    long idle{0};
    long prevTotal{0};
    long total{0};
};

#endif