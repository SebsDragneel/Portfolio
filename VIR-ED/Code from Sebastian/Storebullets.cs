﻿using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System;
using UnityEngine.SceneManagement;
using TMPro;

public class Storebullets : MonoBehaviour
{
    public ProteinCollectorScript PCS;
    public GameObject press;
    int WhatToRead = 0;
    public static int amount;
    public static int stingamount;

    public GameObject EventSystem;
    public GameObject CGAMPspawnSystem;
    public GameObject StingspawnSystem;

    public static int bulletamount;
    public static int numberofstingsdone;
    public static int neededstings = 5;
    public GameObject theScore, scoreBoard, UI, BulletAmount, TheLevel;
    public GameObject Username, ProfilePic;

    public GameObject theLives;
    public TextMeshPro subtitles;
    public static float finalScore = 0;
    public float score = 0;
    public float ReturnScore() { return score; }
    public void AddToScore(float _score) { score += _score; }
    private int lives = 3;
    int level = 1;

    public GameObject shotSpawn;
    public float tumble = 5;
    public float fireRate;
    public GameObject bullet;
    public bool fin;
    private float nextFire;

    private string[] texts =
        {
        "Welcome to CGamp Snatcher",
        "Your objective is to grab cGAMP and direct them towards the STING molecules on the Endoplasmic Recticulum.",
        "This is how your cells communicate that a virus has been detected. If you look around, there are CGamp molecules in the area. Just look at them to pick them up. Try to get 10.",
        //(After the user gets 10 Cgamps)
        "Perfect! Now help them reach the STING molecules by shooting them. Press the button to shoot and make sure they don't collide with other objects.",
        //(After hitting 10 stings)
        "Awesome! Remember that CGamp can spawn behind you.",
        //(end)
        };
    private bool last = false, text = false, finish = false;

    void Awake()
    {
        PCS.StartHazards();
    }

    public void LoseresetPos()
    {
        if (GlobalVariables.arcadeMode == true)
        {
            lives--;
        }
    }

    void ShowScore()
    {
        fin = true;
        if (score > finalScore)
            finalScore = score;
        if (finalScore > PlayerPrefs.GetFloat("CGampScore"))
            PlayerPrefs.SetFloat("CGampScore", finalScore);
        else
            finalScore = PlayerPrefs.GetFloat("CGampScore");

        UI.SetActive(false);
        scoreBoard.SetActive(true);
        CGAMPspawnSystem.GetComponent<SpawnCGamp>().enabled = false;
        StingspawnSystem.GetComponent<SpawnSting>().enabled = false;
        scoreBoard.transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z + 1);
        lives = 3;
        Username.GetComponent<TMPro.TextMeshPro>().text = FacebookManager.Instance.ProfileName + ": " + score.ToString();
        if (FacebookManager.Instance.ProfilePic != null)
            ProfilePic.GetComponent<Image>().sprite = FacebookManager.Instance.ProfilePic;
    }

    public void RestartGame()
    {
        UI.SetActive(true);
        scoreBoard.SetActive(false);
        lives = 3;
        level = 1;
        score = 0;
        theLives.GetComponent<TMPro.TextMeshPro>().text = "LIVES: " + lives;
    }

    void Start()
    {
        // BannerScript.LockTrophy("Endoplasmic reticulum");

        amount = 0;
        stingamount = 0;
        fin = false;
        score = 0;
        bulletamount = 0;
        BulletAmount.GetComponent<TMPro.TextMeshPro>().text = "CGamp: " + bulletamount;
        theLives.GetComponent<TMPro.TextMeshPro>().text = "LIVES: " + lives;
        TheLevel.GetComponent<TMPro.TextMeshPro>().text = "LEVEL: " + level;
        if (GlobalVariables.arcadeMode == false)
        {
            TheLevel.SetActive(false);
            theScore.SetActive(false);
        }
        if (GlobalVariables.tutorial)
            Click();
    }

    void Update()
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Set up how turtorial will show players basic gameplay
        if (GlobalVariables.tutorial == true)
        {
            if ((WhatToRead == 4 && bulletamount >= 10) || (WhatToRead == 6 && numberofstingsdone >= 1))
                Click();

            bool held = Input.GetButton("Fire1");
            if (held && !last)
            {
                if (text)
                {
                    finish = true;
                }
                else
                {
                    press.SetActive(false);
                    if ((WhatToRead != 4 && WhatToRead != 6))
                        Click();
                }
            }
            last = held;
        }

        BulletAmount.GetComponent<TextMeshPro>().text = "CGamp: " + bulletamount;
        bool bPressed = Input.GetButtonDown("Fire1");
        //     bool bHeld = Input.GetButton("Fire1");
        //     bool bUp = Input.GetButtonUp("Fire1");

        if (bPressed && Time.time > nextFire)
        {
            // if (SoundManager.IsJordanPlaying("28860__junggle__btn050") == false)
            SoundManager.PlayJordanVoice("link3");
            shootCGamp();
        }
    }

    void FixedUpdate()
    {

        if (score >= 1000)
        {
            BannerScript.UnlockTrophy("Endoplasmic reticulum");
        }

        if (GlobalVariables.arcadeMode == false)
        {
            if (numberofstingsdone >= 20)
            {
                CellGameplayScript.loadCase = 2;
                Set.SetAndEnterStatic(15);
            }
        }

        if (GlobalVariables.arcadeMode == true)
        {
            theLives.GetComponent<TMPro.TextMeshPro>().text = "LIVES: " + lives;
            if (lives < 1)
            {
                if (GlobalVariables.tutorial)
                {
                    Set.SetAndEnterStatic(5);
                }
                else
                {
                    ShowScore();
                }
            }
            if (numberofstingsdone >= neededstings)
            {
                TheLevel.GetComponent<TMPro.TextMeshPro>().text = "Level: " + level;
                neededstings += 5;
            }
            int tmp = (int)score;
            theScore.GetComponent<TMPro.TextMeshPro>().text = "SCORE: " + tmp.ToString();
        }
        else
        {
            theScore.GetComponent<TMPro.TextMeshPro>().text = "";
            TheLevel.GetComponent<TMPro.TextMeshPro>().text = "";
            theLives.GetComponent<TMPro.TextMeshPro>().text = "";
        }
    }

    private void shootCGamp()
    {
        if (bullet)
        {
            if (bulletamount > 0)
            {
                nextFire = Time.time + fireRate;
                GameObject obj = Instantiate(bullet, shotSpawn.transform.position, shotSpawn.transform.rotation) as GameObject;
                obj.GetComponent<Rigidbody>().angularVelocity = UnityEngine.Random.insideUnitSphere * tumble;

                bulletamount -= 1;
                BulletAmount.GetComponent<TMPro.TextMeshPro>().text = "CGamp: " + bulletamount;
            }
        }
    }

    private void Click()
    {
        switch (WhatToRead)
        {
            case 0:
                {
                    EventSystem.SetActive(false);
                    SoundManager.PlayCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-001");
                    StartCoroutine(TurnTextOn(0));
                }
                break;
            case 1:
                {
                    if (SoundManager.IsCellVoicePlaying("Medical_VR_CGAMP_Snatcher_Tutorial_Line-001"))
                        SoundManager.StopCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-001");
                    SoundManager.PlayCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-002");
                    StartCoroutine(TurnTextOn(1));
                }
                break;
            case 2:
                {
                    if (SoundManager.IsCellVoicePlaying("Medical_VR_CGAMP_Snatcher_Tutorial_Line-002"))
                        SoundManager.StopCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-002");
                    SoundManager.PlayCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-003");
                    StartCoroutine(TurnTextOn(2));
                }
                break;
            case 3:
                if (SoundManager.IsCellVoicePlaying("Medical_VR_CGAMP_Snatcher_Tutorial_Line-003"))
                    SoundManager.StopCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-003");
                EventSystem.SetActive(true);
                subtitles.text = " ";
                break;
            case 4:
                {
                    SoundManager.PlayCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-004");
                    StartCoroutine(TurnTextOn(3));
                }
                break;
            case 5:
                if (SoundManager.IsCellVoicePlaying("Medical_VR_CGAMP_Snatcher_Tutorial_Line-004"))
                    SoundManager.StopCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-004");
                subtitles.text = " ";

                break;
            case 6:
                SoundManager.PlayCellVoice("Medical_VR_CGAMP_Snatcher_Tutorial_Line-005");
                StartCoroutine(TurnTextOn(4));
                break;
            default:
                subtitles.text = "";
                GlobalVariables.tutorial = false;
                if (GlobalVariables.arcadeMode == true)
                {
                    GlobalVariables.arcadeMode = true;
                    SceneManager.LoadScene("CGampSnatcher");

                }
                else
                {
                    GlobalVariables.arcadeMode = false;
                    SceneManager.LoadScene("CGampSnatcher");
                }
                break;
        }
        WhatToRead++;
    }

    #region Text
    IEnumerator TurnTextOn(int index)
    {
        while (text)
            yield return 0;

        text = true;
        subtitles.text = "_";

        while (subtitles.text != texts[index] && !finish)
        {
            yield return new WaitForSeconds(GlobalVariables.textDelay);

            if (subtitles.text.Length == texts[index].Length)
            {
                subtitles.text = texts[index];
            }
            else
            {
                subtitles.text = subtitles.text.Insert(subtitles.text.Length - 1, texts[index][subtitles.text.Length - 1].ToString());
            }
        }
        subtitles.text = texts[index];
        finish = false;
        text = false;
        press.SetActive(true);
    }
    #endregion
}