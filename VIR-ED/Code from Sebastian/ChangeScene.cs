﻿using UnityEngine;
using UnityEngine.SceneManagement;

public static class ChangeScene
{
    public static int index = 0;

    public static void LoadScene(string scene)
    {
        //SoundManager.PlaySFX("MenuEnter");
        SceneManager.LoadScene(scene);
    }

    public static void Exit()
    {
        //SoundManager.PlaySFX("MenuEnter");
        Application.Quit();
    }

    public static void EnterEvent()
    {
        GlobalVariables.arcadeMode = true;
        switch (index)
        {
            case 0:
                LoadScene("Strategy");
                break;
            case 1:
                LoadScene("MemoryGame");
                break;
            case 2:
                LoadScene("FightVirus");
                break;
            case 3:
                LoadScene("DodgeAntibodies");
                break;
            case 4:
                LoadScene("SimonDNA");
                break;
            case 5:
                LoadScene("CGampSnatcher");
                break;
            case 6:
                LoadScene("ATPGTPShooter");
                break;
            case 7:
                SoundManager.DestroySFX();
                LoadScene("MainMenu");
                break;
            case 8:
                Exit();
                break;
            case 9:
                LoadScene("MinigameMenu");
                break;
            case 10:
                LoadScene("TrophyRoom");
                break;
            case 11:
                LoadScene("Credits");
                break;
            case 12:
                LoadScene("OptionsMenu");
                break;
            case 13:
                LoadScene("DestroyTheCell");
                break;
            case 14:
                if (SceneManager.GetActiveScene().name.Contains("MainMenu"))
                {
                    VirusGameplayScript.loadCase = 0;
                    GlobalVariables.arcadeMode = false;
                }
                LoadScene("VirusGameplay");
                break;
            case 15:
                if (SceneManager.GetActiveScene().name.Contains("MainMenu"))
                {
                    CellGameplayScript.loadCase = 0;
                    GlobalVariables.arcadeMode = false;
                }
                LoadScene("CellGameplay");
                break;
        }
    }

    public static void SetIndex(int anything)
    {
        index = anything;
    }
}



