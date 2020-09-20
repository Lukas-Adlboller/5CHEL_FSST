#include <iostream>
#include <stdio.h>

#define FILE_BUFFER_SIZE 2048
#define END_CHARACTER ';'

typedef enum ProgramStates { Initialize, Numbers, Characters, SpecialCharacters, CleanUp, Finished } ProgramStates;

FILE* openFile(const char* filePath, const char* fileAccess);
void closeFile(FILE* fileHandler);
int readFile(FILE* fileHandler, char* buffer, int bufferSize);
int countWithCharRange(char* buffer, const char rangeStart, const char rangeEnd, const char endChar);
int countTotalChars(char* buffer, const char endChar);

ProgramStates currentState;

int main()
{
  const char* filePath = "C:\\Users\\lukas\\source\\repos\\5CHEL_FSST_StateMachine\\Debug\\TestFile.txt";
  char* fileBuffer = new char[FILE_BUFFER_SIZE];

  FILE* fileHandler = NULL;

  int countedNumbers = 0;
  int countedCharacters = 0;
  int countedSpecialCharacters = 0;

  currentState = Initialize;
  while (currentState != Finished)
  {
    switch (currentState)
    {
    case Initialize:
      fileHandler = openFile(filePath, "r");
      if (readFile(fileHandler, fileBuffer, FILE_BUFFER_SIZE) == -1)
      {
        printf("File '%s' not found.", filePath);
        return -1;
      }

      currentState = Numbers;
      break;
    case Numbers:
      countedNumbers = countWithCharRange(fileBuffer, '0', '9', END_CHARACTER);
      currentState = Characters;
      break;
    case Characters:
      countedCharacters = countWithCharRange(fileBuffer, 'A', 'Z', END_CHARACTER) +
        countWithCharRange(fileBuffer, 'a', 'z', END_CHARACTER);
      currentState = SpecialCharacters;
      break;
    case SpecialCharacters:
      countedSpecialCharacters = countTotalChars(fileBuffer, END_CHARACTER) - countedNumbers - countedCharacters;
      currentState = CleanUp;
      break;
    case CleanUp:
      closeFile(fileHandler);
      currentState = Finished;
      break;
    }
  }

  printf("Buffer:\t\t%s\nNumbers:\t%d\nChars:\t\t%d\nSpecials:\t%d\nEnd Char:\t%c\n",
    fileBuffer,
    countedNumbers,
    countedCharacters,
    countedSpecialCharacters,
    END_CHARACTER);
  return 0;
}

/// <summary>
/// Opens a file and returns a pointer to the handler of that file.
/// </summary>
/// <param name="filePath">Path to file</param>
/// <param name="fileAccess">Access type of file</param>
/// <returns>Pointer to handler of a file</returns>
FILE* openFile(const char* filePath, const char* fileAccess)
{
  FILE* fileHandler;

  fopen_s(&fileHandler, filePath, fileAccess);
  return fileHandler;
}

/// <summary>
/// Closes a file.
/// </summary>
/// <param name="fileHandler">Pointer to handler of a file.</param>
void closeFile(FILE* fileHandler)
{
  fclose(fileHandler);
}

/// <summary>
/// Reads content of a file and writes it in a char buffer.
/// </summary>
/// <param name="fileHandler">Pointer to handler of a file</param>
/// <param name="buffer">Buffer where data will be written into</param>
/// /// <param name="bufferSize">Size of buffer</param>
/// <returns>Error Status</returns>
int readFile(FILE* fileHandler, char* buffer, int bufferSize)
{
  if (fileHandler == NULL)
  {
    return -1;
  }

  int bufferIndex = 0;
  while ((buffer[bufferIndex] = fgetc(fileHandler)) != EOF)
  {
    if (bufferIndex == bufferSize - 2)
    {
      break;
    }

    bufferIndex++;
  }

  buffer[bufferIndex + 1] = '\0';

  return 0;
}

/// <summary>
/// Counts chars in a buffer which are in a user defined range.
/// </summary>
/// <param name="buffer">Buffer</param>
/// <param name="rangeStart">Start of range</param>
/// <param name="rangeEnd">End of range</param>
/// <param name="endChar">End char</param>
/// <returns>Counted chars in range</returns>
int countWithCharRange(char* buffer, const char rangeStart, const char rangeEnd, const char endChar)
{
  int bufferIndex = 0;
  int countedChars = 0;

  char currentChar;
  while ((currentChar = buffer[bufferIndex]) != endChar)
  {
    if (currentChar >= rangeStart && currentChar <= rangeEnd)
    {
      countedChars++;
    }

    bufferIndex++;
  }

  return countedChars;
}

/// <summary>
/// Counts all chars till a certain end character.
/// </summary>
/// <param name="buffer">Buffer</param>
/// <param name="endChar">End char</param>
/// <returns>Counted Chars</returns>
int countTotalChars(char* buffer, const char endChar)
{
  int bufferIndex = 0;
  while (buffer[bufferIndex] != endChar)
  {
    bufferIndex++;
  }

  return bufferIndex;
}