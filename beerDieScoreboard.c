//table layout
#define PLAYER_LEFT 0
#define PLAYER_RIGHT 1

//button interface
#define INC_SCORE 1
#define DEC_SCORE 2

//telemtry
#define TELEM_BAUD 57600

typedef struct Score{
  int Team1[2];
  int Team2[2];
}score;

enum states {
  SCORING,
  TRANSMIT,
  RECIEVE,
} state;

//global variables
struct Score gameScore;
enum states curState = SCORING;
int pressedButtonNumber = 0;
int pressType = 1; //1 for short press (increment), 2 for longer press (decrement)

//function prototypes
void resetScore();
void updateScore();
void incrementScore(int teamNumber);
void decrementScore(int teamNumber);
int getTeamScore(int teamNumber);

void setup() {
  Serial.begin(9600);
  resetScore();
}

void loop() {
  switch(curState) {
    case SCORING:
        //pressedButtonNumber = buttonPressed(); //blocking and waits for user input, also set pressType = 1 for inc and 2 for dec based on length of press
        testCase1();

        //temporary way to build packet for testing
        String packetString = String(getTeamScore(1)) + "," + String(getTeamScore(2)) + "," + String(gameScore.Team1[0]) + "," + String(gameScore.Team1[1]) + "," + String(gameScore.Team2[0]) + "," + String(gameScore.Team2[1]);
        Serial.print(packetString);
        }
    break;
    case TRANSMIT:
    break;
    case RECIEVE:
    break;
    default:
    exit(1);
    break;
  }
}

void testCase1(){ //should results in the game state -> (T1 = 1 point, T2 = 2 points, P1 = 0 points, P2 = 1 point, P3 = 0 points, P4 = 2 points)
  pressedButtonNumber = 1;
  pressType = 1;
  updateScore();
  //Score: 1, 0
  
  pressedButtonNumber = 2;
  pressType = 1;
  updateScore();
  //Score: 2, 0

  pressedButtonNumber = 4;
  pressType = 1;
  updateScore();
  //Score: 2, 1

  pressedButtonNumber = 1;
  pressType = 2;
  updateScore();
  //Score: 1, 1

   pressedButtonNumber = 4;
  pressType = 1;
  updateScore();
  //Score: 1, 2
}

void updateScore(){
   if(pressedButtonNumber <= 2){ //team 1
      if(pressType == INC_SCORE){
         incrementScore(1);
      } else if (pressType == DEC_SCORE){
         decrementScore(1);
      }
   } else if(pressedButtonNumber > 2){ //team 2
      if(pressType == INC_SCORE){
         incrementScore(2);
      } else if (pressType == DEC_SCORE){
         decrementScore(2);
      }  
   }
}

void incrementScore(int teamNumber){
  if(teamNumber == 1){
      gameScore.Team1[pressedButtonNumber-1]++;
  } else if(teamNumber == 2){
      gameScore.Team2[pressedButtonNumber-3]++;
  }
}

void decrementScore(int teamNumber){
  if(teamNumber == 1){
      gameScore.Team1[pressedButtonNumber-1]--;
  } else if(teamNumber == 2){
      gameScore.Team2[pressedButtonNumber-3]--;
  }
}

int getTeamScore(int teamNumber){
  if(teamNumber == 1){
      return (gameScore.Team1[PLAYER_LEFT] + gameScore.Team1[PLAYER_RIGHT]);
  } else if(teamNumber == 2){
      return (gameScore.Team2[PLAYER_LEFT] + gameScore.Team2[PLAYER_RIGHT]);
  }
  return 0;
}

void resetScore(){
  gameScore.Team1[PLAYER_LEFT] = 0;
  gameScore.Team1[PLAYER_RIGHT] = 0;
  gameScore.Team2[PLAYER_LEFT] = 0;
  gameScore.Team2[PLAYER_RIGHT] = 0;
  pressedButtonNumber = 0;
}
