/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "SpriteRenderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"
#include "TileManager.h"

#include "shellapi.h"

#include <Windows.h>

#include <string>

/// Delete the renderer, the object manager, and the tile manager. The renderer
/// needs to be deleted before this destructor runs so it will be done elsewhere.

CGame::~CGame(){
  delete m_pParticleEngine;
  delete m_pObjectManager;
  delete m_pTileManager;
} //destructor

/// Initialize the renderer, the tile manager and the object manager, load 
/// images and sounds, and begin the game.

void CGame::Initialize(){
  m_pRenderer = new LSpriteRenderer(eSpriteMode::Batched2D); 
  m_pRenderer->Initialize(eSprite::Size); 
  LoadImages(); //load images from xml file list
  
  m_pTileManager = new CTileManager((size_t)m_pRenderer->GetWidth(eSprite::Tile));
  m_pObjectManager = new CObjectManager; //set up the object manager 
  LoadSounds(); //load the sounds for this game

  m_pParticleEngine = new LParticleEngine2D(m_pRenderer);

  BeginGame();
} //Initialize

/// Load the specific images needed for this game. This is where `eSprite`
/// values from `GameDefines.h` get tied to the names of sprite tags in
/// `gamesettings.xml`. Those sprite tags contain the name of the corresponding
/// image file. If the image tag or the image file are missing, then the game
/// should abort from deeper in the Engine code leaving you with an error
/// message in a dialog box.

void CGame::LoadImages(){  
  m_pRenderer->BeginResourceUpload();

  m_pRenderer->Load(eSprite::Tile,    "tile"); 
  m_pRenderer->Load(eSprite::Player,  "player");
  m_pRenderer->Load(eSprite::Bullet,  "bullet");
  m_pRenderer->Load(eSprite::Bullet2, "bullet2");
  m_pRenderer->Load(eSprite::Smoke,   "smoke");
  m_pRenderer->Load(eSprite::Spark,   "spark");
  m_pRenderer->Load(eSprite::Turret,  "turret");
  m_pRenderer->Load(eSprite::Line,    "greenline");
  m_pRenderer->Load(eSprite::Spike, "spike");
  m_pRenderer->Load(eSprite::Door, "door");
  m_pRenderer->Load(eSprite::DoorOpen, "dooropen");
  m_pRenderer->Load(eSprite::Star, "star");
  m_pRenderer->Load(eSprite::HealthPack, "healthpack");
  m_pRenderer->Load(eSprite::OneUp, "oneup");
  m_pRenderer->Load(eSprite::Bat, "bat");
  m_pRenderer->Load(eSprite::Swooper, "swooper");
  m_pRenderer->Load(eSprite::LaunchPad, "launchpad");
  m_pRenderer->Load(eSprite::Win, "win");
  m_pRenderer->Load(eSprite::Lose, "lose");
  m_pRenderer->Load(eSprite::HealthBar_Green, "healthbar_green");
  m_pRenderer->Load(eSprite::HealthBar_Red, "healthbar_red");
  m_pRenderer->Load(eSprite::Creeper, "creeper");
  m_pRenderer->Load(eSprite::CreeperExplosion, "creeper_explosion");

  m_pRenderer->Load(eSprite::Walkleft, "walkleft");
  m_pRenderer->Load(eSprite::Walkright, "walkright");
  m_pRenderer->Load(eSprite::Standleft, "standleft");
  m_pRenderer->Load(eSprite::Standright, "standright");
  m_pRenderer->Load(eSprite::Jump, "jump");


  m_pRenderer->Load(eSprite::Walkleft, "walkleft");
  m_pRenderer->Load(eSprite::Walkright, "walkright");
  m_pRenderer->Load(eSprite::Standleft, "standleft");
  m_pRenderer->Load(eSprite::Standright, "standright");
  m_pRenderer->Load(eSprite::Jump, "jump");

  m_pRenderer->Load(eSprite::Grappler, "grappler");
  m_pRenderer->Load(eSprite::Shotgun, "shotgun");

  m_pRenderer->EndResourceUpload();
} //LoadImages

/// Initialize the audio player and load game sounds.

void CGame::LoadSounds(){
  m_pAudio->Initialize(eSound::Size);

  m_pAudio->Load(eSound::Grunt, "grunt");
  m_pAudio->Load(eSound::Clang, "clang");
  m_pAudio->Load(eSound::Gun, "gun");
  m_pAudio->Load(eSound::Ricochet, "ricochet");
  m_pAudio->Load(eSound::Start, "start");
  m_pAudio->Load(eSound::Boom, "boom");
  m_pAudio->Load(eSound::Star, "star");
  m_pAudio->Load(eSound::PowerDown, "powerdown");
  m_pAudio->Load(eSound::Bounce, "bounce");
} //LoadSounds

/// Release all of the DirectX12 objects by deleting the renderer.

void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release

/// Ask the object manager to create a player object and turrets specified by
/// the tile manager.

void CGame::CreateObjects(){
  std::vector<Vector2> turretpos; //vector of turret positions
  Vector2 playerpos; //player position
  std::vector<Vector2> spikepos; //vector of spikepos
  std::vector<Vector2> doorpos;//door position
  std::vector<Vector2> starpos; // star position
  std::vector<Vector2> batpos; // bat position
  std::vector<Vector2> swooperpos; //swooper position
  std::vector<Vector2> launchpadpos; // launchpad pos
  std::vector<Vector2> healthpackpos;
  std::vector<Vector2> oneuppos;
  std::vector<Vector2> shotgunpos;
  std::vector<Vector2> creeperpos;
  m_pTileManager->GetObjects(turretpos, playerpos, spikepos, doorpos, starpos, batpos, launchpadpos, healthpackpos, oneuppos, shotgunpos, creeperpos, swooperpos); //get positions
  
  m_pPlayer = (CPlayer*)m_pObjectManager->create(eSprite::Standright, playerpos);
  m_pGrappler = (CGrappler*)m_pObjectManager->create(eSprite::Grappler, playerpos);     //create grappler on player position

  for(const Vector2& pos: turretpos)
    m_pObjectManager->create(eSprite::Turret, pos);

  for (const Vector2& pos : spikepos)
      m_pObjectManager->create(eSprite::Spike, pos);

  for (const Vector2& pos : doorpos)
      m_pObjectManager->create(eSprite::Door, pos);

  for (const Vector2& pos : starpos)
      m_pObjectManager->create(eSprite::Star, pos);

  for (const Vector2& pos : batpos)
      m_pObjectManager->create(eSprite::Bat, pos);

  for (const Vector2& pos : swooperpos)
      m_pObjectManager->create(eSprite::Swooper, pos);

  for (const Vector2& pos : launchpadpos)
      m_pObjectManager->create(eSprite::LaunchPad, pos);

  for (const Vector2& pos : healthpackpos)
      m_pObjectManager->create(eSprite::HealthPack, pos);

  for (const Vector2& pos : oneuppos)
      m_pObjectManager->create(eSprite::OneUp, pos);

  for (const Vector2& pos : shotgunpos)
      m_pObjectManager->create(eSprite::Shotgun, pos);

  for (const Vector2& pos : creeperpos)
      m_pObjectManager->create(eSprite::Creeper, pos);
} //CreateObjects

/// Call this function to start a new game. This should be re-entrant so that
/// you can restart a new game without having to shut down and restart the
/// program. Clear the particle engine to get rid of any existing particles,
/// delete any old objects out of the object manager and create some new ones.

void CGame::BeginGame(){  
  m_pParticleEngine->clear(); //clear old particles
  state = 2;
  
  switch(m_nNextLevel){
    case 0: m_pTileManager->LoadMap("Media\\Maps\\level_one.txt"); break;
    case 1: m_pTileManager->LoadMap("Media\\Maps\\mario_star_from_china.txt"); break;
    case 2: m_pTileManager->LoadMap("Media\\Maps\\momentum_testing.txt"); break;
    case 3: m_pTileManager->LoadMap("Media\\Maps\\platforms.txt"); break;
    case 4: m_pTileManager->LoadMap("Media\\Maps\\spikechasm.txt"); break;
    case 5: m_pTileManager->LoadMap("Media\\Maps\\launchpads.txt"); break;
    case 6: m_pTileManager->LoadMap("Media\\Maps\\ascension.txt"); break;
    case 7: m_pTileManager->LoadMap("Media\\Maps\\final_climb.txt"); break;
    case 8: m_pTileManager->LoadMap("Media\\Maps\\winner.txt"); state = 0; break;
  } //switch

  m_pObjectManager->clear(); //clear old objects
  CreateObjects(); //create new objects (must be after map is loaded) 
  m_pAudio->stop(); //stop all  currently playing sounds
  m_pAudio->play(eSound::Start); //play start-of-game sound
  m_pGrappler->normalGun(); // set back to normal gun
  m_eGameState = eGameState::Playing; //now playing
} //BeginGame

/// Poll the keyboard state and respond to the key presses that happened since
/// the last frame.

void CGame::KeyboardHandler(){
  m_pKeyboard->GetState(); //get current keyboard state

  if (m_pKeyboard->TriggerDown(VK_RETURN)) {
      m_nNextLevel = (m_nNextLevel + 1) % 9;
      BeginGame();
  } //if
  
  //if(m_pKeyboard->TriggerDown(VK_F1)) //help
  //  ShellExecute(0, 0, "https://larc.unt.edu/code/topdown/", 0, 0, SW_SHOW);
  
  if(m_pKeyboard->TriggerDown(VK_F2)) //toggle frame rate
    m_bDrawFrameRate = !m_bDrawFrameRate;
  
  //if(m_pKeyboard->TriggerDown(VK_F3)) //toggle AABB drawing
  //  m_bDrawAABBs = !m_bDrawAABBs; 

  if(m_pKeyboard->TriggerDown(VK_BACK)) //start game
    BeginGame();

  if(m_pPlayer){ //safety

    if (m_pKeyboard->Down('D')) //strafe right
        m_pPlayer->StrafeRight();
  
    if(m_pKeyboard->Down('A')) //strafe left
        m_pPlayer->StrafeLeft();

    // Twin Stick Controls

    // Left
    if (m_pKeyboard->Down('J') || m_pKeyboard->Down(VK_LEFT)) {
        if (m_pKeyboard->Down('I') || m_pKeyboard->Down(VK_UP)) // UP
        {
            m_pGrappler->AimGun(2.35619f);
        }
        else if (m_pKeyboard->Down('K') || m_pKeyboard->Down(VK_DOWN)) // DOWN
        {
            m_pGrappler->AimGun(3.92699f);
        }
        else
        {
            m_pGrappler->AimGun(3.14159f);
        }
        m_pGrappler->Shoot();
    }
    else if (m_pKeyboard->Down('L') || m_pKeyboard->Down(VK_RIGHT)) // RIGHT
    {
        if (m_pKeyboard->Down('I') || m_pKeyboard->Down(VK_UP)) // UP
        {
            m_pGrappler->AimGun(0.785398f);
        }
        else if (m_pKeyboard->Down('K') || m_pKeyboard->Down(VK_DOWN)) // DOWN
        {
            m_pGrappler->AimGun(5.49778f);
        }
        else
        {
            m_pGrappler->AimGun(0.0f);
        }
        m_pGrappler->Shoot();
    }
    else if (m_pKeyboard->Down('I') || m_pKeyboard->Down(VK_UP)) // UP
    {
        m_pGrappler->AimGun(01.5708f);
        m_pGrappler->Shoot();
    }
    else if (m_pKeyboard->Down('K') || m_pKeyboard->Down(VK_DOWN)) // DOWN
    {
        m_pGrappler->AimGun(4.71239f);
        m_pGrappler->Shoot();
    }

    if (m_pKeyboard->Down(VK_SPACE)) //Jump
        m_pPlayer->Jump();

    //if(m_pKeyboard->TriggerDown('G')) //toggle god mode
    //  m_bGodMode = !m_bGodMode;
  } //if
} //KeyboardHandler

/// Poll the XBox controller state and respond to the controls there.

void CGame::ControllerHandler(){
  if(!m_pController->IsConnected())return;

  m_pController->GetState(); //get state of controller's controls 
  
  if(m_pPlayer){ //safety

    // LEFT STICK MOVEMENT
    Vector2 normalizedLStick = m_pController->GetLThumb();
    if (normalizedLStick.x != 0 && normalizedLStick.y != 0) {
        normalizedLStick.Normalize();
        float angleLStick = std::atan2(normalizedLStick.y, normalizedLStick.x);

        float degreesLStick = angleLStick * (180.0f / M_PI);
        if (degreesLStick < 0)
            degreesLStick += 360.0f; // ensure angle is positive

        if (degreesLStick >= 337.5 || degreesLStick < 22.5) // Move Right
            m_pPlayer->StrafeRight();

        if (degreesLStick >= 157.5 && degreesLStick < 202.5) // Move Left
            m_pPlayer->StrafeLeft();
    }
    else
    {
        if (m_pController->GetDPadRight()) m_pPlayer->StrafeRight();
        if (m_pController->GetDPadLeft()) m_pPlayer->StrafeLeft();
    }

    // RIGHT STICK MOVEMENT
    Vector2 normalizedRStick = m_pController->GetRThumb();
    if (normalizedRStick.x != 0 && normalizedRStick.y != 0) {
        normalizedRStick.Normalize();
        float angleRStick = std::atan2(normalizedRStick.y, normalizedRStick.x);

        float degreesRStick = angleRStick * (180.0f / M_PI);
        if (degreesRStick < 0)
            degreesRStick += 360.0f; // ensure angle is positive

        // Check direction ranges
        if (degreesRStick >= 337.5 || degreesRStick < 22.5) // RIGHT
            m_pGrappler->AimGun(0.0f);
        if (degreesRStick >= 22.5 && degreesRStick < 67.5) // UP RIGHT
            m_pGrappler->AimGun(0.785398f);
        if (degreesRStick >= 67.5 && degreesRStick < 112.5) // UP
            m_pGrappler->AimGun(01.5708f);
        if (degreesRStick >= 112.5 && degreesRStick < 157.5) // UP LEFT
            m_pGrappler->AimGun(2.35619f);
        if (degreesRStick >= 157.5 && degreesRStick < 202.5) // LEFT
            m_pGrappler->AimGun(3.14159f);
        if (degreesRStick >= 202.5 && degreesRStick < 247.5) // DOWN LEFT
            m_pGrappler->AimGun(3.92699f);
        if (degreesRStick >= 247.5 && degreesRStick < 292.5) // DOWN
            m_pGrappler->AimGun(4.71239f);
        if (degreesRStick >= 292.5 && degreesRStick < 337.5) // DOWN RIGHT
            m_pGrappler->AimGun(5.49778f);

        m_pGrappler->Shoot();

        test = degreesRStick;
    }

    if (m_pController->GetRTrigger() || m_pController->GetLTrigger()) {
        m_pPlayer->Jump();
    }

  } //if
} //ControllerHandler

void CGame::MouseHandler() {
    HWND hWnd = GetActiveWindow();
    POINT pointCursorPos;
    GetCursorPos(&pointCursorPos);
    ScreenToClient(hWnd, &pointCursorPos);
    Vector2 cursorPos(pointCursorPos.x, pointCursorPos.y);

    m_pGrappler->cursorPos = cursorPos;
}// MouseHandler

/// Draw the current frame rate to a hard-coded position in the window.
/// The frame rate will be drawn in a hard-coded position using the font
/// specified in `gamesettings.xml`.

void CGame::DrawFrameRateText(){
  const std::string s = std::to_string(m_pTimer->GetFPS()) + " fps"; //frame rate
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText(s.c_str(), pos); //draw to screen
} //DrawFrameRateText

/// Draw the god mode text to a hard-coded position in the window using the
/// font specified in `gamesettings.xml`.

void CGame::DrawGodModeText(){
  const Vector2 pos(64.0f, 30.0f); //hard-coded position
  m_pRenderer->DrawScreenText("God Mode", pos); //draw to screen
} //DrawGodModeText

void CGame::DrawPlayerLivesText() {
    const Vector2 pos(900.0f, 30.0f);
    std::string livesText = "Lives: " + std::to_string(m_nPlayerLives);
    m_pRenderer->DrawScreenText(livesText.c_str(), pos, Colors::White);
}

void CGame::DrawTutorialText() {
    float aboveY = 725.0f;
    float belowY = 490.0f;
    float belowY2 = 445.0f;

    Vector2 pos = ConvertGameToScreenSpace(Vector2(80.0f, aboveY));
    m_pRenderer->DrawScreenText("Runnin' Gunner", pos, Colors::White);

    pos = ConvertGameToScreenSpace(Vector2(80.0f, belowY));
    m_pRenderer->DrawScreenText("Keyboard: Use WASD to move", pos, Colors::White);
    pos = ConvertGameToScreenSpace(Vector2(80.0f, belowY2));
    m_pRenderer->DrawScreenText("Controller: Use the left analog stick to move", pos, Colors::White);

    pos = ConvertGameToScreenSpace(Vector2(1150.0f, belowY));
    m_pRenderer->DrawScreenText("Keyboard: Use space to jump", pos, Colors::White);
    pos = ConvertGameToScreenSpace(Vector2(1090.0f, belowY2));
    m_pRenderer->DrawScreenText("Controller: Use either triggers to jump", pos, Colors::White);

    pos = ConvertGameToScreenSpace(Vector2(2410.0f, belowY2));
    m_pRenderer->DrawScreenText("Keyboard: Use the arrow keys to shoot", pos, Colors::White);
    pos = ConvertGameToScreenSpace(Vector2(2375.0f, belowY2 - 45.0f));
    m_pRenderer->DrawScreenText("Controller: Use the right analog stick to shoot", pos, Colors::White);
    pos = ConvertGameToScreenSpace(Vector2(2375.0f, belowY2 - 90.0f));
    m_pRenderer->DrawScreenText("All enemies must be dead to unlock the door", pos, Colors::White);

    pos = ConvertGameToScreenSpace(Vector2(3710.0f, belowY));
    m_pRenderer->DrawScreenText("Collect powerups to help your journey", pos, Colors::White);
}

void CGame::DrawWinnerText() {
    Vector2 pos = ConvertGameToScreenSpace(Vector2(720.0f, 290.0f));
    m_pRenderer->DrawScreenText("You Win!", pos, Colors::Green);
}

Vector2 CGame::ConvertGameToScreenSpace(Vector2& gameVector) {
    Vector2 screenSpaceVector;

    Vector3 cameraSpacePosition = m_pRenderer->GetCameraPos();
    screenSpaceVector.x = (gameVector.x - cameraSpacePosition.x) + (m_nWinHeight / 1.5f);
    screenSpaceVector.y = ((gameVector.y - cameraSpacePosition.y) * -1.0f) + (m_nWinHeight / 2.0f);

    return screenSpaceVector;
}

/// Ask the object manager to draw the game objects. The renderer is notified of
/// the start and end of the frame so that it can let Direct3D do its
/// pipelining jiggery-pokery.

void CGame::RenderFrame(){                                  //if you want something to happen every frame, it's probably going to happen here
  m_pRenderer->BeginFrame(); //required before rendering

  m_pObjectManager->draw(); //draw objects
  m_pParticleEngine->Draw(); //draw particles
  if(m_bDrawFrameRate)DrawFrameRateText(); //draw frame rate, if required
  if(m_bGodMode)DrawGodModeText(); //draw god mode text, if required
  if (m_nNextLevel == 0) DrawTutorialText();
  if (m_nNextLevel == 8) DrawWinnerText();

  DrawPlayerLivesText();

  if (m_pPlayer != nullptr)
  {
      deathLocation = m_pPlayer->GetPos() + Vector2(0, 64);
  }

  if(state == 0)
  {

  }
  else if (state == 1)
  {
      m_pRenderer->Draw(eSprite::Lose, Vector2(deathLocation), 0.0f);
  }

  /*std::string grapX = std::to_string(m_pGrappler->GetPos().x);
  std::string grapY = std::to_string(m_pGrappler->GetPos().y);
  std::string strcoords = "(" + grapX + ", " + grapY +")";
  char coords[30];
  strcpy(coords, strcoords.c_str());

  m_pRenderer->DrawScreenText("C", m_pGrappler->cursorPos);
  m_pRenderer->DrawScreenText(coords, m_pGrappler->GetPos());*/

  m_pRenderer->EndFrame(); //required after rendering
} //RenderFrame

/// Make the camera follow the player, but don't let it get too close to the
/// edge unless the world is smaller than the window, in which case we just
/// center everything.

void CGame::FollowCamera(){
  if(m_pPlayer == nullptr)return; //safety

  Vector3 vCameraPos(m_pPlayer->GetPos()); //player position

  if(m_vWorldSize.x > m_nWinWidth){ //world wider than screen
    vCameraPos.x = std::max(vCameraPos.x, m_nWinWidth/2.0f); //stay away from the left edge
    vCameraPos.x = std::min(vCameraPos.x, m_vWorldSize.x - m_nWinWidth/2.0f);  //stay away from the right edge
  } //if
  else vCameraPos.x = m_vWorldSize.x/2.0f; //center horizontally.
  
  if(m_vWorldSize.y > m_nWinHeight){ //world higher than screen
    vCameraPos.y = std::max(vCameraPos.y, m_nWinHeight/2.0f);  //stay away from the bottom edge
    vCameraPos.y = std::min(vCameraPos.y, m_vWorldSize.y - m_nWinHeight/2.0f); //stay away from the top edge
  } //if
  else vCameraPos.y = m_vWorldSize.y/2.0f; //center vertically

  m_pRenderer->SetCameraPos(vCameraPos); //camera to player
} //FollowCamera

/// This function will be called regularly to process and render a frame
/// of animation, which involves the following. Handle keyboard input.
/// Notify the audio player at the start of each frame so that it can prevent
/// multiple copies of a sound from starting on the same frame.  
/// Move the game objects. Render a frame of animation. 

void CGame::ProcessFrame(){
  KeyboardHandler(); //handle keyboard input
  ControllerHandler(); //handle controller input
  MouseHandler(); // handle mouse position
  m_pAudio->BeginFrame(); //notify audio player that frame has begun
  
  m_pTimer->Tick([&](){ //all time-dependent function calls should go here
    m_pObjectManager->move(); //move all objects
    FollowCamera(); //make camera follow player
    m_pParticleEngine->step(); //advance particle animation
    if (m_pPlayer) {
        m_pPlayer->playerLogic(); // run player logic
    }
  });

  RenderFrame(); //render a frame of animation
  ProcessGameState(); //check for end of game
} //ProcessFrame

/// Take action appropriate to the current game state. If the game is currently
/// playing, then if the player has been killed or all turrets have been
/// killed, then enter the wait state. If the game has been in the wait
/// state for longer than 3 seconds, then restart the game.

void CGame::ProcessGameState(){
  static float t = 0; //time at start of game

  switch(m_eGameState){
    case eGameState::Playing:
      if (m_pPlayer && m_pObjectManager->GetNumEnemies() == 0) {
          m_pObjectManager->SetDoorOpen();
      }

      if(m_pPlayer == nullptr || m_pPlayer->m_bIsWinner == true){
        m_eGameState = eGameState::Waiting; //now waiting
        t = m_pTimer->GetTime(); //start wait timer
        if (m_pPlayer == nullptr)
        {
            state = 1;
        }
      } //if

      if (m_pPlayer != nullptr && m_pPlayer->GetHasOneUp()) {
          m_nPlayerLives++;
          m_pPlayer->ClearHasOneUp();
      }
      break;

    case eGameState::Waiting:
        if (m_pPlayer == nullptr) // if player is dead
        {
            if (m_pTimer->GetTime() - t > 3.0f) // let player dead animation play out
            {
                m_nPlayerLives--;
                if (m_nPlayerLives > 0) {
                    BeginGame(); // restart level from beginning
                }
                else {
                    m_nNextLevel = 0;
                    m_nPlayerLives = 3;
                    BeginGame();
                }
                
            } 
        }
        else // if not...
        {
            if (m_pPlayer->m_bIsWinner == true) { //player won
                m_pPlayer->m_bIsWinner == false;
                m_nNextLevel = (m_nNextLevel + 1) % 9; //note: 4 instead of 3
                BeginGame(); //restart game
            }
        } //if
      break;
  } //switch

  if (m_pPlayer != nullptr && m_pPlayer->hasShotgun())
  {
      m_pGrappler->setShotgun();
  }

} //CheckForEndOfGame

