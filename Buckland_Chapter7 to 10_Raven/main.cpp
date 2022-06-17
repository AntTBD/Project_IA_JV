#pragma warning (disable:4786)
#include <windowsx.h>
#include <time.h>
#include "constants.h"
#include "misc/utils.h"
#include "Time/PrecisionTimer.h"
#include "Resource.h"
#include "misc/windowutils.h"
#include "misc/Cgdi.h"
#include "debug/DebugConsole.h"
#include "Raven_UserOptions.h"
#include "Raven_Game.h"
#include "lua/Raven_Scriptor.h"

//need to include this for the toolbar stuff
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#define READY                       1

// DEFAULT PARAMTERS FOR PARAMETERS DIALOG -----------------------------------------

#define DEATH_MATCH_MAX_BOT         25
#define DEATH_MATCH_MIN_BOT         2
#define DEATH_MATCH_INIT_BOT        7

#define TEAM_MATCH_MAX_BOT          20
#define TEAM_MATCH_MIN_BOT          1
#define TEAM_MATCH_INIT_BOT         5

#define BATTLEROYALE_MATCH_MAX_BOT          30
#define BATTLEROYALE_MATCH_MIN_BOT          2
#define BATTLEROYALE_MATCH_INIT_BOT         5


// LIST FOR DIALOG CONTROL ---------------------------------------------------------

std::map<int, std::pair <const char*, Raven_Game::Mode>> MAP_GRP_BOXES_LIST
{ 
    {ID_GRPBOX_DEATHMATCH,      std::make_pair( "Death Match",   Raven_Game::Mode::DeathMatch   )},
    {ID_GRPBOX_TEAMMATCH,       std::make_pair( "Team Match",    Raven_Game::Mode::Team         )},
    {ID_GRPBOX_ONEVSONE,        std::make_pair( "1 VS 1",        Raven_Game::Mode::oneVSone     )},
    {ID_GRPBOX_BATTLEROYALE,    std::make_pair( "Battle Royale", Raven_Game::Mode::BattleRoyale )},
};

const std::list<int> ID_GRPBOX_DEATHMATCH_ITEMS = {
    ID_GRPBOX_DEATHMATCH,
    ID_DEATHMATCH_TEXT_NB,
    ID_DEATHMATCH_SPIN_TEXT,
    ID_DEATHMATCH_SPIN,
    ID_DEATHMATCH_SPIN_BOX,

};

const std::list<int> ID_GRPBOX_TEAMMATCH_ITEMS = {
    ID_GRPBOX_TEAMMATCH,
    ID_TEAM_TEXT_1,
    ID_TEAM_TEXT_2,
    ID_TEAM_SPIN_TEAM_1,
    ID_TEAM_SPIN_TEAM_2,
    ID_TEAM_SPIN_TEAM_1_TEXT,
    ID_TEAM_SPIN_TEAM_2_TEXT,
    ID_TEAM_SPIN_TEAM_1_BOX,
    ID_TEAM_SPIN_TEAM_2_BOX
};

const std::list<int> ID_GRPBOX_ONEVSONE_ITEMS = {
    ID_GRPBOX_ONEVSONE,

};

const std::list<int> ID_GRPBOX_BATTLEROYAL_ITEMS = {
    ID_GRPBOX_BATTLEROYALE,
    ID_BATTLEROYALE_TEXT,
    ID_BATTLEROYALE_SPIN_BOX,
    ID_BATTLEROYALE_SPIN,
};

const std::list<int> ID_GRPBOX_LEARNING_ITEMS = {
    ID_GRPBOX_LEARNING,
    ID_RADIO_BOT,
    ID_RADIO_HUMAN,
};

//--------------------------------- Globals ------------------------------
//------------------------------------------------------------------------

HINSTANCE hinstance;
HWND static mainwnd;

char* g_szApplicationName = "Raven - ";
char* g_szWindowClassName = "MyWindowClass";


Raven_Game* g_pRaven;

Raven_Game::Mode match_mode = Raven_Game::Mode::DeathMatch;
bool human_playing = false;
bool has_learning_bots = false;
bool learn_from_human = false;

// DEATHMATCH 
int bot_number_deathmatch = DEATH_MATCH_INIT_BOT;

// TEAM DEATH MATCH
int bot_number_team_1 = TEAM_MATCH_INIT_BOT;
int bot_number_team_2 = TEAM_MATCH_INIT_BOT;

// DEATHMATCH 
int bot_number_battleroyale = BATTLEROYALE_MATCH_INIT_BOT;

void RegisterModalDialogClass(HWND);
void SwitchMatchMode(HWND hwndDlg, std::map<int, std::pair <const char*, Raven_Game::Mode>>::const_iterator it);
BOOL CALLBACK DlgParamProc(HWND, UINT, WPARAM, LPARAM);
void OpenDialog(HWND);
Raven_Game* CreateGame();

//---------------------------- WindowProc ---------------------------------
//	
//	This is the callback function which handles all the windows messages
//-------------------------------------------------------------------------

LRESULT CALLBACK WindowProc (HWND   hwnd,
                             UINT   msg,
                             WPARAM wParam,
                             LPARAM lParam)
{
 
   //these hold the dimensions of the client window area
   static int cxClient, cyClient; 

   //used to create the back buffer
   static HDC		hdcBackBuffer;
   static HBITMAP	hBitmap;
   static HBITMAP	hOldBitmap;

   //to grab filenames
   static TCHAR   szFileName[MAX_PATH],
                  szTitleName[MAX_PATH];


    switch (msg)
    {
		//A WM_CREATE msg is sent when your application window is first
		//created
    case WM_CREATE:
      {
        hinstance = GetModuleHandle(NULL);

        RegisterModalDialogClass(hwnd);
        
        OpenDialog(hwnd);

        //to get get the size of the client window first we need  to create
        //a RECT and then ask Windows to fill in our RECT structure with
        //the client window size. Then we assign to cxClient and cyClient 
        //accordingly
        RECT rect;

        GetClientRect(hwnd, &rect);

        cxClient = rect.right;
        cyClient = rect.bottom;

        //seed random number generator
        srand((unsigned)time(NULL));


        //---------------create a surface to render to(backbuffer)

        //create a memory device context
        hdcBackBuffer = CreateCompatibleDC(NULL);

        //get the DC for the front buffer
        HDC hdc = GetDC(hwnd);

        hBitmap = CreateCompatibleBitmap(hdc,
            cxClient,
            cyClient);


        //select the bitmap into the memory device context
        hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

        //don't forget to release the DC
        ReleaseDC(hwnd, hdc);

        //make sure the menu items are ticked/unticked accordingly
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_NAVGRAPH, UserOptions->m_bShowGraph);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_PATH, UserOptions->m_bShowPathOfSelectedBot);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_IDS, UserOptions->m_bShowBotIDs);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_QUICK, UserOptions->m_bSmoothPathsQuick);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_PRECISE, UserOptions->m_bSmoothPathsPrecise);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_HEALTH, UserOptions->m_bShowBotHealth);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_TARGET, UserOptions->m_bShowTargetOfSelectedBot);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_FOV, UserOptions->m_bOnlyShowBotsInTargetsFOV);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_SCORES, UserOptions->m_bShowScore);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_GOAL_Q, UserOptions->m_bShowGoalsOfSelectedBot);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_INDICES, UserOptions->m_bShowNodeIndices);
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_SENSED, UserOptions->m_bShowOpponentsSensedBySelectedBot);

      }

      break;
    case WM_KEYDOWN:
    {

        g_pRaven->HandleKeyDown(wParam);

    }
    break;
    case WM_KEYUP:
      {
        switch(wParam)
        {
         case VK_ESCAPE:
          {
             OpenDialog(hwnd);
          }
          
          break;

         case 'P':

           g_pRaven->TogglePause();

           break;

         case '1':

           g_pRaven->ChangeWeaponOfPossessedBot(type_blaster);

           break;

         case '2':

           g_pRaven->ChangeWeaponOfPossessedBot(type_shotgun);

           break;
           
         case '3':

           g_pRaven->ChangeWeaponOfPossessedBot(type_rocket_launcher);

           break;

         case '4':

           g_pRaven->ChangeWeaponOfPossessedBot(type_rail_gun);

           break;

         case 'X':

           g_pRaven->ExorciseAnyPossessedBot();

           break;


         case VK_UP:

           g_pRaven->AddBots(1); break;

         case VK_DOWN:

           g_pRaven->RemoveBot(); break;
           

        }
      }
    break;


    case WM_LBUTTONDOWN:
    {
      g_pRaven->ClickLeftMouseButton(MAKEPOINTS(lParam));
    }
    break;

   case WM_RBUTTONDOWN:
    {
      g_pRaven->ClickRightMouseButton(MAKEPOINTS(lParam));
    }
    break;

    case WM_COMMAND:
    {

     switch(wParam)
      {
      

      case IDM_GAME_LOAD:
          
          FileOpenDlg(hwnd, szFileName, szTitleName, "Raven map file (*.map)", "map");

          debug_con << "Filename: " << szTitleName << "";

          if (strlen(szTitleName) > 0)
          {
            g_pRaven->LoadMap(szTitleName);
          }

          break;

      case IDM_GAME_ADDBOT:

          g_pRaven->AddBots(1);
          
          break;

      case IDM_GAME_REMOVEBOT:
          
          g_pRaven->RemoveBot();

          break;

      case IDM_GAME_PAUSE:

          g_pRaven->TogglePause();

          break;

      case IDM_GAME_MAIN_MENU:

          OpenDialog(hwnd);

          break;

      case IDM_GAME_QUIT:
          PostQuitMessage(0);
          break;



      case IDM_NAVIGATION_SHOW_NAVGRAPH:

        UserOptions->m_bShowGraph = !UserOptions->m_bShowGraph;

        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_NAVGRAPH, UserOptions->m_bShowGraph);

        break;
        
      case IDM_NAVIGATION_SHOW_PATH:

        UserOptions->m_bShowPathOfSelectedBot = !UserOptions->m_bShowPathOfSelectedBot;

        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_PATH, UserOptions->m_bShowPathOfSelectedBot);

        break;

      case IDM_NAVIGATION_SHOW_INDICES:

        UserOptions->m_bShowNodeIndices = !UserOptions->m_bShowNodeIndices;

        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SHOW_INDICES, UserOptions->m_bShowNodeIndices);

        break;

      case IDM_NAVIGATION_SMOOTH_PATHS_QUICK:

        UserOptions->m_bSmoothPathsQuick = !UserOptions->m_bSmoothPathsQuick;
        UserOptions->m_bSmoothPathsPrecise = false;
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_PRECISE, UserOptions->m_bSmoothPathsPrecise);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_QUICK, UserOptions->m_bSmoothPathsQuick);

        break;

      case IDM_NAVIGATION_SMOOTH_PATHS_PRECISE:

        UserOptions->m_bSmoothPathsPrecise = !UserOptions->m_bSmoothPathsPrecise;
        UserOptions->m_bSmoothPathsQuick = false;
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_QUICK, UserOptions->m_bSmoothPathsQuick);
        CheckMenuItemAppropriately(hwnd, IDM_NAVIGATION_SMOOTH_PATHS_PRECISE, UserOptions->m_bSmoothPathsPrecise);

        break;

      case IDM_BOTS_SHOW_IDS:

        UserOptions->m_bShowBotIDs = !UserOptions->m_bShowBotIDs;

        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_IDS, UserOptions->m_bShowBotIDs);

        break;

      case IDM_BOTS_SHOW_HEALTH:

        UserOptions->m_bShowBotHealth = !UserOptions->m_bShowBotHealth;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_HEALTH, UserOptions->m_bShowBotHealth);

        break;

      case IDM_BOTS_SHOW_TARGET:

        UserOptions->m_bShowTargetOfSelectedBot = !UserOptions->m_bShowTargetOfSelectedBot;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_TARGET, UserOptions->m_bShowTargetOfSelectedBot);

        break;

      case IDM_BOTS_SHOW_SENSED:

        UserOptions->m_bShowOpponentsSensedBySelectedBot = !UserOptions->m_bShowOpponentsSensedBySelectedBot;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_SENSED, UserOptions->m_bShowOpponentsSensedBySelectedBot);

        break;


      case IDM_BOTS_SHOW_FOV:

        UserOptions->m_bOnlyShowBotsInTargetsFOV = !UserOptions->m_bOnlyShowBotsInTargetsFOV;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_FOV, UserOptions->m_bOnlyShowBotsInTargetsFOV);

        break;

      case IDM_BOTS_SHOW_SCORES:

        UserOptions->m_bShowScore = !UserOptions->m_bShowScore;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_SCORES, UserOptions->m_bShowScore);

        break;

      case IDM_BOTS_SHOW_GOAL_Q:

        UserOptions->m_bShowGoalsOfSelectedBot = !UserOptions->m_bShowGoalsOfSelectedBot;
        
        CheckMenuItemAppropriately(hwnd, IDM_BOTS_SHOW_GOAL_Q, UserOptions->m_bShowGoalsOfSelectedBot);

        break;

      }//end switch
    }
     break;
    
    case WM_PAINT:
      {
 		       
         PAINTSTRUCT ps;
          
         BeginPaint (hwnd, &ps);

        //fill our backbuffer with white
         BitBlt(hdcBackBuffer,
                0,
                0,
                cxClient,
                cyClient,
                NULL,
                NULL,
                NULL,
                WHITENESS);
          
         
         gdi->StartDrawing(hdcBackBuffer);

         if (g_pRaven != NULL)
             g_pRaven->Render();

         gdi->StopDrawing(hdcBackBuffer);


         //now blit backbuffer to front
		 BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 
          
         EndPaint (hwnd, &ps);

      }
      break;

    //has the user resized the client area?
		case WM_SIZE:
		  {
        //if so we need to update our variables so that any drawing
        //we do using cxClient and cyClient is scaled accordingly
			  cxClient = LOWORD(lParam);
			  cyClient = HIWORD(lParam);

        //now to resize the backbuffer accordingly. First select
        //the old bitmap back into the DC
		SelectObject(hdcBackBuffer, hOldBitmap);

        //don't forget to do this or you will get resource leaks
        DeleteObject(hBitmap); 

		//get the DC for the application
        HDC hdc = GetDC(hwnd);

		//create another bitmap of the same size and mode
        //as the application
        hBitmap = CreateCompatibleBitmap(hdc,
											  cxClient,
											  cyClient);

		ReleaseDC(hwnd, hdc);
			  
		//select the new bitmap into the DC
        SelectObject(hdcBackBuffer, hBitmap);

      }

      break;
          
		 case WM_DESTROY:
	     {

         //clean up our backbuffer objects
         SelectObject(hdcBackBuffer, hOldBitmap);

         DeleteDC(hdcBackBuffer);
         DeleteObject(hBitmap); 
         

         // kill the application, this sends a WM_QUIT message  
				 PostQuitMessage (0);
		 }

       break;

     }//end switch

     //this is where all the messages not specifically handled by our 
     //winproc are sent to be processed
	 return DefWindowProc (hwnd, msg, wParam, lParam);
}

Raven_Game* CreateGame()
{
    Raven_Game* game;

    game = new Raven_Game(
        bot_number_deathmatch,
        bot_number_team_1,
        bot_number_team_2,
        bot_number_battleroyale,
        match_mode,
        human_playing,
        has_learning_bots,
        learn_from_human
    );

    return game;
}

//-------------------------------- Utils --------------------------------------

void RegisterModalDialogClass(HWND hwnd) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = (WNDPROC)DlgParamProc;
    wc.hInstance = hinstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpszClassName = TEXT("DialogClass");
    RegisterClassEx(&wc);
}

int EnableGroupItemsWithGroupID(HWND hwndDlg, int ID, bool enable)
{
    std::list<int> list;

    switch (ID)
    {
    case ID_GRPBOX_DEATHMATCH:
        list = ID_GRPBOX_DEATHMATCH_ITEMS;
        break;
    case ID_GRPBOX_TEAMMATCH:
        list = ID_GRPBOX_TEAMMATCH_ITEMS;
        break;
    case ID_GRPBOX_ONEVSONE:
        list = ID_GRPBOX_ONEVSONE_ITEMS;
        break;
    case ID_GRPBOX_BATTLEROYALE:
        list = ID_GRPBOX_BATTLEROYAL_ITEMS;
        break;
    case ID_GRPBOX_LEARNING:
        list = ID_GRPBOX_LEARNING_ITEMS;
        break;
    default:
        return NULL;
    }

    std::list<int>::const_iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        EnableWindow(GetDlgItem(hwndDlg, *it), enable);
    }

    return 1;
}

void EnableMatchModeWithID(HWND hwndDlg, int ID_MATCH)
{
    std::map<int, std::pair <const char*, Raven_Game::Mode>>::const_iterator it;

    for (it = MAP_GRP_BOXES_LIST.begin(); it != MAP_GRP_BOXES_LIST.end(); ++it) {
        int c_id = it->first;

        if (ID_MATCH == c_id)
        {
            EnableGroupItemsWithGroupID(hwndDlg, c_id, true);
        }
        else
        {
            EnableGroupItemsWithGroupID(hwndDlg, c_id, false);
        }
    }
}

void SwitchMatchMode(HWND hwndDlg, std::map<int, std::pair <const char*, Raven_Game::Mode>>::const_iterator it)
{
    EnableMatchModeWithID(hwndDlg, it->first);

    match_mode = it->second.second;

    std::string title = g_szApplicationName + std::string(it->second.first);
    SetWindowText(mainwnd, title.c_str());
}

void OpenDialog(HWND hwnd) {

    int dialog_message = DialogBox(hinstance, "PARAM_DIALOG", hwnd, DlgParamProc);

    if (dialog_message == NULL)
    {
        PostQuitMessage(0);
    }

    //create the game
    g_pRaven = CreateGame();

    if (g_pRaven == nullptr)
    {
        MessageBox(hwnd, "An error occured with the game creation. Please Try Again.", "Unexpected Error", MB_ICONERROR);
    }
}

void HandleGameStatus(HWND hWnd, int status)
{
    if (status == 1)
    {
        MessageBox(hWnd, g_pRaven->GetVictoryMessage().c_str(), "Victoire", MB_ICONINFORMATION);
        OpenDialog(hWnd);
    }
}
void InitGameParameters() {
    match_mode = Raven_Game::Mode::DeathMatch;
    human_playing = false;
    has_learning_bots = false;
    learn_from_human = false;

    // DEATHMATCH 
    bot_number_deathmatch = DEATH_MATCH_INIT_BOT;

    // TEAM DEATH MATCH
    bot_number_team_1 = TEAM_MATCH_INIT_BOT;
    bot_number_team_2 = TEAM_MATCH_INIT_BOT;

    // DEATHMATCH 
    bot_number_battleroyale = BATTLEROYALE_MATCH_INIT_BOT;
}
//------------------------------- PARAM DIALOG --------------------------------

BOOL CALLBACK DlgParamProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    mainwnd = GetParent(hwndDlg);
    TCHAR* words = TEXT("Raven - Game Main Menu !");
    HWND static staticbox;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        
        InitGameParameters();

        staticbox = GetDlgItem(hwndDlg, 1);
        SetWindowText(staticbox, words);
        
        std::map<int, std::pair <const char*, Raven_Game::Mode>>::const_iterator it;

        for (it = MAP_GRP_BOXES_LIST.begin(); it != MAP_GRP_BOXES_LIST.end(); ++it) 
        {
            SendDlgItemMessage(hwndDlg, ID_MATCH_TYPE, CB_ADDSTRING, 0, (LONG)it->second.first);
        }
        SendDlgItemMessage(hwndDlg, ID_MATCH_TYPE, CB_SETCURSEL, 0, 0);
        
        SendDlgItemMessage(hwndDlg, ID_DEATHMATCH_SPIN, UDM_SETRANGE, 0, MAKELPARAM(DEATH_MATCH_MAX_BOT, DEATH_MATCH_MIN_BOT));
        SendDlgItemMessage(hwndDlg, ID_DEATHMATCH_SPIN, UDM_SETPOS, 0, DEATH_MATCH_INIT_BOT);

        SendDlgItemMessage(hwndDlg, ID_TEAM_SPIN_TEAM_1, UDM_SETRANGE, 0, MAKELPARAM(TEAM_MATCH_MAX_BOT, TEAM_MATCH_MIN_BOT));
        SendDlgItemMessage(hwndDlg, ID_TEAM_SPIN_TEAM_1, UDM_SETPOS, 0, TEAM_MATCH_INIT_BOT);

        SendDlgItemMessage(hwndDlg, ID_TEAM_SPIN_TEAM_2, UDM_SETRANGE, 0, MAKELPARAM(TEAM_MATCH_MAX_BOT, TEAM_MATCH_MIN_BOT));
        SendDlgItemMessage(hwndDlg, ID_TEAM_SPIN_TEAM_2, UDM_SETPOS, 0, TEAM_MATCH_INIT_BOT);

        SendDlgItemMessage(hwndDlg, ID_BATTLEROYALE_SPIN, UDM_SETRANGE, 0, MAKELPARAM(BATTLEROYALE_MATCH_MAX_BOT, BATTLEROYALE_MATCH_MIN_BOT));
        SendDlgItemMessage(hwndDlg, ID_BATTLEROYALE_SPIN, UDM_SETPOS, 0, BATTLEROYALE_MATCH_INIT_BOT);

        CheckDlgButton(hwndDlg, ID_RADIO_BOT, !learn_from_human);


        //HFONT hFont = CreateFont(1, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
        //SendMessage(hwndDlg, WM_SETFONT, (WPARAM)hFont, 0);

        //Graphic update of the dialog
        EnableMatchModeWithID(hwndDlg, ID_GRPBOX_DEATHMATCH);
        EnableGroupItemsWithGroupID(hwndDlg, ID_GRPBOX_LEARNING, false);
        //Set the default match mode as "Death Match"
        match_mode = MAP_GRP_BOXES_LIST.begin()->second.second;
        //Change the app named accordingly
        std::string title = g_szApplicationName + std::string(MAP_GRP_BOXES_LIST.begin()->second.first);
        SetWindowText(mainwnd, title.c_str());

        return TRUE;
    }

    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
            case CBN_SELCHANGE:
            {
                int selection = SendMessage(
                    GetDlgItem(hwndDlg, ID_MATCH_TYPE),
                    CB_GETCURSEL, NULL, NULL
                );

                // Get key with nb
                std::map<int, std::pair <const char*, Raven_Game::Mode>>::const_iterator it = MAP_GRP_BOXES_LIST.begin();

                for (int i = 0; i < selection; i++)
                {
                    it++;
                }
                EnableMatchModeWithID(hwndDlg, it->first);

                match_mode = it->second.second;

                std::string title = g_szApplicationName + std::string(it->second.first);
                SetWindowText(mainwnd, title.c_str());

            }
            break;

            case BN_CLICKED:
            {
                // Human checkbox 
                if (LOWORD(wParam) == ID_HUMAN_CHECK) {
                    human_playing = SendMessage(
                        GetDlgItem(hwndDlg, ID_HUMAN_CHECK),
                        BM_GETCHECK, NULL, NULL
                    ); 
                }

                // Learning Bot checkbox 
                if (LOWORD(wParam) == ID_LEARNING_CHECK) {
                    has_learning_bots = IsDlgButtonChecked(hwndDlg, ID_LEARNING_CHECK);
                    EnableGroupItemsWithGroupID(hwndDlg, ID_GRPBOX_LEARNING, has_learning_bots);
                }

                // Radio 
                // Learning Bot checkbox 
                if (LOWORD(wParam) == ID_RADIO_BOT) {
                    learn_from_human = false;
                }
                if (LOWORD(wParam) == ID_RADIO_HUMAN) {
                    learn_from_human = true;
                }

                // Start button pressed
                if (LOWORD(wParam) == ID_START_BUTTON) {
                    
                    int mode_selected = SendMessage(
                        GetDlgItem(hwndDlg, ID_MATCH_TYPE),
                        CB_GETCURSEL, NULL, NULL
                    );
                    
                    EndDialog(hwndDlg, READY);
                }

                // Quit button pressed
                if (LOWORD(wParam) == ID_QUIT_BTN) {
                    PostQuitMessage(0);
                }
            }
            break;

            case EN_CHANGE:
            {
                if (LOWORD(wParam) == ID_DEATHMATCH_SPIN_BOX) {

                    bot_number_deathmatch = GetDlgItemInt(hwndDlg, ID_DEATHMATCH_SPIN_BOX, false, false);
                    
                    if (bot_number_deathmatch > DEATH_MATCH_MAX_BOT)
                    {
                        bot_number_deathmatch = DEATH_MATCH_MAX_BOT;
                        SetDlgItemText(hwndDlg, ID_DEATHMATCH_SPIN_BOX, std::to_string(bot_number_deathmatch).c_str());
                    }
                }

                if (LOWORD(wParam) == ID_TEAM_SPIN_TEAM_1_BOX) {

                    bot_number_team_1 = GetDlgItemInt(hwndDlg, ID_TEAM_SPIN_TEAM_1_BOX, false, false);

                    if (bot_number_team_1 > TEAM_MATCH_MAX_BOT)
                    {
                        bot_number_team_1 = TEAM_MATCH_MAX_BOT;
                        SetDlgItemText(hwndDlg, ID_TEAM_SPIN_TEAM_1_BOX, std::to_string(bot_number_team_1).c_str());
                    }
                }

                if (LOWORD(wParam) == ID_TEAM_SPIN_TEAM_2_BOX) {

                    bot_number_team_2 = GetDlgItemInt(hwndDlg, ID_TEAM_SPIN_TEAM_2_BOX, false, false);

                    if (bot_number_team_2 > TEAM_MATCH_MAX_BOT)
                    {
                        bot_number_team_2 = TEAM_MATCH_MAX_BOT;
                        SetDlgItemText(hwndDlg, ID_TEAM_SPIN_TEAM_2_BOX, std::to_string(bot_number_team_2).c_str());
                    }
                }

                if (LOWORD(wParam) == ID_BATTLEROYALE_SPIN_BOX) {

                    bot_number_battleroyale = GetDlgItemInt(hwndDlg, ID_BATTLEROYALE_SPIN_BOX, false, false);

                    if (bot_number_battleroyale > BATTLEROYALE_MATCH_MAX_BOT)
                    {
                        bot_number_battleroyale = BATTLEROYALE_MATCH_MAX_BOT;
                        SetDlgItemText(hwndDlg, ID_BATTLEROYALE_SPIN_BOX, std::to_string(bot_number_battleroyale).c_str());
                    }
                }
            }
            break;
        }
        break;
    
    case WM_CLOSE:
        EndDialog(hwndDlg, NULL);//destroy dialog window
        return FALSE;
        break;
    }
    return 0;
}

//-------------------------------- WinMain -------------------------------
//
//	The entry point of the windows program
//------------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     szCmdLine, 
                    int       iCmdShow)
{
  MSG msg;
  //handle to our window
	HWND						hWnd;

 //the window class structure
	WNDCLASSEX     winclass;

  // first fill in the window class stucture
  winclass.cbSize        = sizeof(WNDCLASSEX);
  winclass.style         = CS_HREDRAW | CS_VREDRAW;
  winclass.lpfnWndProc   = WindowProc;
  winclass.cbClsExtra    = 0;
  winclass.cbWndExtra    = 0;
  winclass.hInstance     = hInstance;
  winclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  winclass.hbrBackground = NULL;
  winclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
  winclass.lpszClassName = g_szWindowClassName;
  winclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  //register the window class
  if (!RegisterClassEx(&winclass))
  {
		MessageBox(NULL, "Registration Failed!", "Error", MB_ICONERROR);

	    //exit the application
		return 0;
  }
		

  try
  {  		 
		 //create the window and assign its ID to hwnd    
     hWnd = CreateWindowEx (NULL,                 // extended style
                            g_szWindowClassName,  // window class name
                            g_szApplicationName,  // window caption
                            WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,  // window style
                            GetSystemMetrics(SM_CXSCREEN)/2 - WindowWidth/2,
                            GetSystemMetrics(SM_CYSCREEN)/2 - WindowHeight/2,                    
                            WindowWidth,     // initial x size
                            WindowHeight,    // initial y size
                            NULL,                 // parent window handle
                            NULL,                 // window menu handle
                            hInstance,            // program instance handle
                            NULL);                // creation parameters


     //make sure the window creation has gone OK
     if(!hWnd)
     {
       MessageBox(NULL, "CreateWindowEx Failed!", "Error!", MB_ICONERROR);
       PostQuitMessage(0);
     }

     
    //make the window visible
    ShowWindow (hWnd, iCmdShow);
    UpdateWindow (hWnd);
   
    //create a timer
    PrecisionTimer timer(FrameRate);

    //start the timer
    timer.Start();

    //enter the message loop
    bool bDone = false;
    int status = -1;

    while(!bDone)
    {
      while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
      {
        if( msg.message == WM_QUIT ) 
        {
          // Stop loop if it's a quit message
	        bDone = true;
        } 

        else 
        {
          TranslateMessage( &msg );
          DispatchMessage( &msg );
        }
      }

      if (timer.ReadyForNextFrame() && msg.message != WM_QUIT)
      {
        status = g_pRaven->Update();
        
        HandleGameStatus(hWnd, status);

        //render 
        RedrawWindow(hWnd);
      }

      //give the OS a little time
      Sleep(2);
     					
    }//end while

  }//end try block

  catch (const std::runtime_error& err)
  {
    ErrorBox(std::string(err.what()));
    //tidy up
    delete g_pRaven;
    UnregisterClass( g_szWindowClassName, winclass.hInstance );
    return 0;
  }
  
 //tidy up
 UnregisterClass( g_szWindowClassName, winclass.hInstance );
 delete g_pRaven;
 return msg.wParam;
}
