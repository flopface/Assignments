// Assignment 1 - Transition energy - Tom Melichar - 01/02/2024
// Code to calculate the energy of a photon emitted from an electron moving between energy levels

#include<iostream>
#include<sstream>
#include<cmath>
#include<iomanip>

const double hydrogen_ionisation{13.5984}, electron_charge{1.60218e-19};

int validate_input(int min_value, int max_value, std::string question, bool no_max)
{
	int value;
	while(true) 
  {
		std::cout<<question; 
    std::cin>>value;
		if(!std::cin.fail() && (std::cin.peek() == '\n')) // checks if ONLY an integer was inputted
    {
      if(value>=min_value) 
      {
        if(!(no_max))
        {
          if(value <= max_value) return value;
          else std::cerr<<"ERROR : Value must be lower than or equal to "<<max_value<<", try again.\n";
        } 
        else return value;
      } 
      else std::cerr<<"ERROR : Value must be greater than or equal to "<<min_value<<", try again.\n";
    } 
    else std::cerr<<"ERROR : Value must be an integer, try again.\n";
		std::cin.clear(); // reset input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty input
	}
}

bool two_option_choice(std::string question, std::string option_1, std::string option_2) 
{
    std::string input;
    while (true) 
    {
      std::cout<<question;
      std::cin>>input;
      if(input == option_1 && std::cin.peek() == '\n') return true; // checks if ONLY option_1 was inputted
      else if(input == option_2 && std::cin.peek() == '\n') return false; // checks if ONLY option_2 was inputted
      else std::cout<<"ERROR : Invalid input. Please enter "<<option_1<<" or "<<option_2<<".\n";
      std::cin.clear(); // reset input
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // empty input
    }
}

double calculate_photon_energy(int Z, int n_i, int n_j) {return hydrogen_ionisation * pow(Z, 2) * (1.0 / pow(n_j, 2) - 1.0 / pow(n_i, 2));}

int main()
{
  double delta_e;
  bool convert, repeat;
  int atomic_number, n_initial, n_final, places;
  while(true) 
  {
    atomic_number = validate_input(1, 118, "\nPlease enter an atomic number: ", false); // 1 <= Z <= 118
    n_initial = validate_input(2, 2, "\nPlease enter an intitial quantum number: ", true); // 2 <= n_i
    n_final = validate_input(1, n_initial-1, "\nPlease enter a final quantum number: ", false); // 1 <= n_j < n_i
    convert = two_option_choice("\nWould you like the energy change in Joules (J) or Electron Volts (e): ", "J", "e"); // j or eV
    places = validate_input(1, 6, "\nHow many significant figures would you like? ", false); // 1 <= sf <= 6
    delta_e = calculate_photon_energy(atomic_number, n_initial, n_final);

    if(convert) std::cout<<"\nEnergy change = "<<std::setprecision(places)<<delta_e * electron_charge<<" J\n\n";
    else std::cout<<"\nEnergy change = "<<std::setprecision(places)<<delta_e<<" eV\n\n";

    repeat = two_option_choice("again? (y/n) ", "y", "n");
    if(!repeat) return 0;
  }
}