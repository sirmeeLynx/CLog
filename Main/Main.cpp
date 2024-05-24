#include "Benchmark.h"
#include "clog.h"
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN true
#else
#define OS_WIN false
#endif

namespace fs = std::filesystem;

//declare ANSI Colors escape code for std::out
const char* RED = "\033[31m";
const char* GREEN = "\033[32m";
const char* CYAN = "\033[38;5;14m";
const char* RESET = "\033[0m"; // Reset color to default


void startUX();
void displayUXBanner();
void displayChoices();
uint8_t getChoice(const std::string &prompt, const std::vector<std::string> &firstLevelChoices);
void processUserInputs(const std::vector<uint8_t> &userChoices, const std::string &message);

void createLogFolderAndFile() {
    // Specify the folder path
    fs::path folderPath = "LOGS";

    // Create the folder if it doesn't exist
    if (fs::exists(folderPath)) return;
    
	fs::create_directory(folderPath);

    // Create and open the text file
    fs::path filePath = folderPath / "LOG.txt";
    std::ofstream logFile(filePath);

    if (logFile.is_open()) {
        std::cout << "Folder and file created successfully." << std::endl;
    } else {
        std::cerr << "Error creating the file." << std::endl;
    }
}

template <typename T>
T getUserInput(const std::string &prompt, T range = std::numeric_limits<T>::min())
{
    T input;
    bool wrongInputEntered = false;
    do
    {
        const auto foreColor = wrongInputEntered ? RED : RESET; // RED or RESET
        std::cout << foreColor << (wrongInputEntered ? "Wrong input entered!!! " : "") << prompt << RESET;
        std::string strInput;
        std::getline(std::cin, strInput);

        try
        {
			//std::cout << std::is_same_v<T, uint8_t> << std::endl;
            if constexpr (std::is_same_v<T, uint8_t>)
            {
                input = static_cast<T>(std::stoi(strInput));
                if (input > 0 && input <= range)
                    break; // Valid input, exit the loop
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                input = strInput;
                break; // Valid input, exit the loop
            }
            wrongInputEntered = true;
        }
        catch (const std::exception &)
        {
            wrongInputEntered = true;
        }

        std::cout << "\033[1A\033[K"; // Clear the current line
    } 
	while (wrongInputEntered);

    return input;
}

void startUX()
{
	std::string prompt = "\n\nWould you like to try the playground again? Enter [Y] or any key to end the demo: ";
	do{
		std::cout << "\033[2J\033[1;1H"; // ANSI escape sequence to clear the screen
		if (!OS_WIN)
		{
			displayUXBanner();
		}
		displayChoices();
	}
	while(getUserInput<std::string>(prompt) == "Y");
}

void displayUXBanner()
{
	//print UX banner to console
	std::printf("\n");
	std::cout << GREEN << "╦ ╦┌─┐┬  ┌─┐┌─┐┌┬┐┌─┐  ┌┬┐┌─┐  ╔═╗╦  ┌─┐┌─┐  ╔═╗┬  ┌─┐┬ ┬┌─┐┬─┐┌─┐┬ ┬┌┐┌┌┬┐" << RESET << std::endl;
	std::cout << GREEN << "║║║├┤ │  │  │ ││││├┤    │ │ │  ║  ║  │ ││ ┬  ╠═╝│  ├─┤└┬┘│ ┬├┬┘│ ││ ││││ ││" << RESET << std::endl;
	std::cout << GREEN << "╚╩╝└─┘┴─┘└─┘└─┘┴ ┴└─┘   ┴ └─┘  ╚═╝╩═╝└─┘└─┘  ╩  ┴─┘┴ ┴ ┴ └─┘┴└─└─┘└─┘┘└┘─┴┘" << RESET << std::endl;
	std::printf("\n");
}

void displayChoices()
{
	const std::vector<std::string> firstLevelChoices{"File","Console","Http"};
	const std::vector<std::string> secondLevelChoices{"Synchronous", "Asynchronous"};
	const std::vector<std::string> thirdLevelChoices{"info","error","warning","debug","critical"};
	const std::string prompt1 = "\nWhere would you like to write your log message?";
	const std::string prompt2 = "\nWhat thread policy would you like to try out?";
	const std::string prompt3 = "\nWhat log severity level would you like to try out?";
	std::map<std::string, std::vector<std::string>> possibleChoices{
		{prompt1, firstLevelChoices},
		{prompt2, secondLevelChoices},
		{prompt3, thirdLevelChoices}	
	};
	const std::vector<std::string> choiceOrder{prompt1,prompt2,prompt3};
	std::vector<uint8_t> userChoices;
	uint8_t counter = 0;
	for(const auto& prompt : choiceOrder)
	{
		const auto choices = possibleChoices[prompt];
		auto choiceNumber = getChoice(prompt, choices);
		if (choiceNumber <= choices.size())
		{
			userChoices.push_back(choiceNumber - 1);
		}

		// switch to the next prompt view
		if (userChoices.size() < choiceOrder.size())
		{
			//clear choices.size() + 3 lines
			uint8_t fixedLines = 4;
			uint8_t linesToClear = static_cast<uint8_t>(choices.size()) + fixedLines;
			for(uint8_t line = 0; line < linesToClear; line++)
			{
				std::cout << "\033[1A\033[K"; //Move cursor up and clear line
			}
		}
	}

	//promp user for their log message
	auto logMessage = getUserInput<std::string>("\nEnter your log message as expressive as you want: ");
	processUserInputs(userChoices, logMessage);
}

uint8_t getChoice(const std::string &prompt, const std::vector<std::string> &firstLevelChoices)
{
	auto continueIt = false;
	uint8_t choiceNumber;
    std::cout << CYAN << prompt.c_str() << RESET << std::endl;
	int8_t counter;
	for (counter = 0; counter < firstLevelChoices.size(); counter++)
	{
		const auto choice = firstLevelChoices[counter];
		//std::cout << GREEN << counter + 1 << ". " << choice.c_str() << RESET << std::endl;
		std::printf("%s%d. %s%s\n", GREEN, counter + 1, choice.c_str(), RESET);
	}
	std::printf("\n");

	choiceNumber = getUserInput<uint8_t>("Enter the number for your selection(ex: 1,2,3..): ", counter);
	return choiceNumber;
}

void processUserInputs(const std::vector<uint8_t> &userChoices, const std::string &message)
{
	//prep console output
	std::printf("\n\n");
	/*
		possible combinations of inputs

		00 - Synchronous File Logger
		01 - Asynchronous File Logger
		10 - Synchronous Basic Logger
		11 - Asynchronous Basic Logger
		20 - Synchrounous Http Logger (currently not supported)
		21 - Asynchronous Http Logger (currently not supported)
	*/
	using logger_factory = std::function<std::shared_ptr<CLog::interface::logger>()>;
	std::string logFilePath = "LOGS/LOG.txt";
	std::map<std::string, logger_factory> loggerMap{
		{"00", [logFilePath](){ return std::make_shared<CLog::file_logger_mt>("SYNC_FILE_LOGGER", logFilePath.c_str()); }},
		{"01", [logFilePath](){ return std::make_shared<CLog::async_file_logger_mt>("ASYNC_FILE_LOGGER", logFilePath.c_str()); }},
		{"10", [](){ return std::make_shared<CLog::console_logger_mt>("SYNC_CONSOLE_LOGGER"); }},
		{"11", [](){ return std::make_shared<CLog::async_console_logger_mt>("ASYNC_CONSOLE_LOGGER"); }},
		{"20", [](){ return std::make_shared<CLog::http_logger_mt>("SYNC_HTTP_LOGGER"); }},
		{"21", [](){ return std::make_shared<CLog::async_http_logger_mt>("ASYNC_HTTP_LOGGER"); }}
	};

	const auto firstLevelInput = userChoices[0];
	const auto secondLevelInput = userChoices[1];
	const auto thirdLevelInput = userChoices[2];
	const auto key = std::to_string(firstLevelInput) + std::to_string(secondLevelInput);

	if (loggerMap.find(key) != loggerMap.end())
	{
		const auto logger = loggerMap[key]();
		std::vector<std::function<void(const std::string &)>> loggerActions{
			[logger](const std::string &message)
			{ logger->info(message.c_str()); },
			[logger](const std::string &message)
			{ logger->error(message.c_str()); },
			[logger](const std::string &message)
			{ logger->warning(message.c_str()); },
			[logger](const std::string &message)
			{ logger->debug(message.c_str()); },
			[logger](const std::string &message)
			{ logger->critical(message.c_str()); }
		};

		// configure logger
		logger->set_level_filter(CLog::level::info | CLog::level::debug | CLog::level::critical | CLog::level::warning | CLog::level::error);
		
		// Color attributes will be ignored for the file sink
		logger->set_pattern(".red([%T] [%n]) .level([%l]): .cyan(%v)");

		//get requested severity level and emit message to sink
		const auto logMessage = loggerActions[thirdLevelInput];
		
		//start critical section
		{
			// start timer and log message
			Timer timer("timer");
			if (logMessage)
				logMessage(message);
		}
	}
	else
	{
		std::cout << RED << "Operation currently unsupported!!!" << RESET << std::endl;
	}
}

int main()
{
	createLogFolderAndFile();
	startUX();

	return 0;
}
