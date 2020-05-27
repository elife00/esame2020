#include </home/elisabetta-ferri/projects/myproject/esame2020/epidemic.hpp>


int main()
{
  int dim = 10;
  double pInf = 0.15;
  double pGua = 0.1;  // 1 giorno = 100 milliseconds
  double ratInf = 0.2;

  Population population(dim);
  std::cout << "How an epidemic begin:\n";

  population.infection(ratInf);
  population.draw();
  population.count();

  std::cout << '\n' << "Epidemic has began\n";

  std::this_thread::sleep_for(std::chrono::seconds(5));

  int days = 0;
  while (true) {
    ++days;
    std::cout << "\033c";
    population = population.epidemic(pInf, pGua);
    population.draw();

    if (population.count()) {
      break;
    }
    std::cout << "giorni dall'inizio dell'epidemia: " << days << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  population.trend();
  std::cout << '\n' << '\n';
  population.parameters();
}