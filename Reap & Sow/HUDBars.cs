using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using UnityEngine.SceneManagement;

class HUDBars : MonoBehaviour {
    PlayerMovement playerMovement;                              // Reference to the player's movement.
    public Image healthSlider;                                 // Reference to the UI's SoalOMeter bar.
    public Image overchargeSlider;                             // Reference to the UI's Overcharge bar.
    public Image LastResortIcon;                                //Refrence to the UI's LastResort icon
    public Image LastResortClock;
    public Image LastResortClockHand;
    public Image TimeSlowVenue;                                 //Refrence to the UI's Blue Time slow screen
    public Image OverchargeScythe;
    public Text OmegaUnavailable;
    Animator AniClockHand;
    Animator Ani;                                              //Refrence to my Player animator
    PlayerAttacks PlayerAtt;                                  //Refrence to attacks so he doesn't attack while in last chance
    public GameObject SacrificeHealth = null;

    public bool lastchanceActive = false;
    public bool LastchanceAvailable = true;                              //iF last chance is avalible. this is the players last chance move once a life

    SoundManager.SavedBGM LastChancePrevBGM;

    float MaxHealth = 1000;                                  // The Max amount of health the player can have.
    float MaxOvercharge = 800;                                // The Max amount of overcharge energy the player can have.
    public float currentHealth;                                    // The current health the player has.
    float currentOvercharge;                               // The current overcharge energy the player has.
    [SerializeField]
    float LastChanceTime = 5;              //How long the player stays in last chance mode
    float LastChanceSoundTime = 2;
    float LCSoundTimer, LCTimer;

    public bool IsOverCharged = false;
    public bool PlayerIsDead;                                                // Whether the player is dead.
    public bool PlayerIsDamaged = false;                                               // True when the player gets damaged.

    public string deathClip;                               // The name of the audio clip to play when the player dies.
    public string Sachealth;     //the name of the audio clip that plays when player sacrifices health.
    public string OverchargeBegin;
    public string[] OverchargeDuring;
    public string OverchargeEnd;
    public string damageClip;

    public Image damageImage;                                   // Reference to an image to flash on the screen on being hurt.
    public float DamageflashSpeed = 120f;                               // The speed the damageImage will fade at.
    float currentflash = 0.0f;
    Color DamageflashColor = new Color(1f, 1f, 1f, 0.5f);     // The colour the damageImage is set to, to flash.

    public Image OverchargeImage;                                   // Reference to an image to flash on the screen on being hurt.
    public float OverchargeflashSpeed = 120f;                               // The speed the damageImage will fade at.
    Color OverchargeflashColor = new Color(1f, 1f, 1f, 0.5f);     // The colour the damageImage is set to, to flash.
    [SerializeField]
    int OverchargeAnimatorLayer = 1;

    public string FireHitSound = "nullsound", FireDeathSound = "nullsound";

    void Awake() {
        // Setting up the references.
        playerMovement = GetComponent<PlayerMovement>();
        Ani = GetComponent<Animator>();
        AniClockHand = LastResortClockHand.GetComponent<Animator>();
        PlayerAtt = GetComponent<PlayerAttacks>();
        OverchargeScythe.gameObject.SetActive(false);
        // Set the initial health of the player.
        currentHealth = MaxHealth;
        //Set the initial overcharge energy of the player.
        currentOvercharge = 0;


        //Disabling Images
        LastResortClock.enabled = false;
        LastResortClockHand.enabled = false;
        TimeSlowVenue.gameObject.SetActive(false);

        Color alpha = OmegaUnavailable.color;
        alpha.a = 0;
        OmegaUnavailable.color = alpha;
        EventManager.OnPause += Pause;
        EventManager.OnUnpause += Unpause;


    }

    void Pause() {
        AniClockHand.enabled = false;
    }

    void Unpause() {
        AniClockHand.enabled = true;
    }

    void OnDestroy() {
        EventManager.OnPause -= Pause;
        EventManager.OnUnpause -= Unpause;
    }

    void Update() {
        if (EventManager.IsPaused()) {
            return;
        }

        if (LCSoundTimer > 0) {
            LCSoundTimer -= Time.deltaTime;
            if (LCSoundTimer <= 0) {
                PlaySounds();
                StartDelayedClock();
            }
        }
        if (LCTimer > 0) {
            LCTimer -= Time.deltaTime;
            if (LCTimer <= 0) {
                EndLastChance();
                ResetTime();
            }
        }


        if (lastchanceActive) {
            PlayerAtt.AttackInProgress = true;
        }
        if (currentHealth <= 0 && PlayerIsDead == false)
            Death();

        if (currentOvercharge >= MaxOvercharge && !IsOverCharged) {
            IsOverCharged = true;
            currentOvercharge = MaxOvercharge;
            SoundManager.PlaySFX(OverchargeBegin);
            SoundManager.SetBGMPitch(-1);
            Ani.SetLayerWeight(OverchargeAnimatorLayer, 1);
        }

        if (IsOverCharged == true && !lastchanceActive) {
            for (int i = 0; i < OverchargeDuring.Length; i++) {
                if (!SoundManager.IsSFXPlaying(OverchargeDuring[i])) {
                    SoundManager.PlaySFX(OverchargeDuring[i]);
                }
            }

            OverchargeScythe.gameObject.SetActive(true);
            currentOvercharge -= 2;

            OverchargeImage.color = OverchargeflashColor;
            currentflash = 0.0f;
        }

        if (currentOvercharge <= 0 && IsOverCharged) {
            currentOvercharge = 0;
            IsOverCharged = false;
            OverchargeScythe.gameObject.SetActive(false);
            SoundManager.PlaySFX(OverchargeEnd);
            SoundManager.SetBGMPitch(1);
            Ani.SetLayerWeight(OverchargeAnimatorLayer, 0);
            for (int i = 0; i < OverchargeDuring.Length; i++) {
                if (SoundManager.IsSFXPlaying(OverchargeDuring[i])) {
                    SoundManager.stopSFX(OverchargeDuring[i]);
                }
            }
        }

        // If the player has just been damaged...
        if (PlayerIsDamaged) {
            // ... set the colour of the damageImage to the flash colour.
            damageImage.color = DamageflashColor;
            currentflash = 0.0f;
        } else {
            // ... transition the colour back to clear.

            currentflash = currentflash + 1.0f / DamageflashSpeed;
            DamageflashColor = Color.Lerp(damageImage.color, Color.clear, currentflash);
            OverchargeflashColor = Color.Lerp(OverchargeImage.color, Color.clear, currentflash);

            damageImage.color = DamageflashColor;
            OverchargeImage.color = OverchargeflashColor;
            DamageflashColor = new Color(1f, 1f, 1f, 0.5f);
            OverchargeflashColor = new Color(1f, 1f, 1f, 0.5f);


        }

        // Reset the damaged flag.
        PlayerIsDamaged = false;

        if (Input.GetKey(KeyCode.Space) && currentHealth > 10 && !IsOverCharged && !lastchanceActive) {
            Instantiate(SacrificeHealth, transform.position, transform.rotation);
            if(!SoundManager.IsSFXPlaying(Sachealth)) {
                SoundManager.PlaySFX(Sachealth);
            }
            int mTemp = 1;
            if (currentHealth > 9 * MaxHealth / 20 && currentHealth <= 4 * MaxHealth / 5) {
                currentHealth = currentHealth - 4 * mTemp;
                currentOvercharge = currentOvercharge + 3 * mTemp;

            } else {
                currentHealth = currentHealth - 2 * mTemp;
                currentOvercharge = currentOvercharge + mTemp;
            }

        }

        // Set the health bar's value to the current health.
        healthSlider.fillAmount = currentHealth / 1000;
        overchargeSlider.fillAmount = currentOvercharge / 800;

    }

    void OnCollisionEnter2D(Collision2D other) {
        if (other.gameObject.tag == "Enemy" && lastchanceActive) {
            other.gameObject.SendMessage("TakeLastChanceDamage", 60);
        }
    }

    public void AddHealth(int NumOfSouls) {

        //SCORE/MULTIPLIER EMP
        EventManager.GatherSouls(NumOfSouls);
        if (!IsOverCharged) {
            if (GetRange() == 1) {
                currentHealth += 3 * NumOfSouls;
            } else {
                currentHealth += NumOfSouls;

            }

            if (currentHealth >= MaxHealth) {
                float oversouls = currentHealth - MaxHealth;
                currentHealth = MaxHealth;
                currentOvercharge += oversouls;
            }
        } else {
            if (GetRange() == 1) {
                currentOvercharge += NumOfSouls * 2;
            } else {
                currentOvercharge += NumOfSouls;
            }
        }
    }

    public void TakeDamage(int amount) {
        if (!IsOverCharged && !lastchanceActive) {

            // Set the damaged flag so the screen will flash.
            PlayerIsDamaged = true;

            // Reduce the current health by the damage amount.
            currentHealth -= amount;

            // Play the hurt sound effect.
            if (SoundManager.IsSFXPlaying(damageClip) == false)
                SoundManager.PlaySFX(damageClip);
            //playerAudio.PlayOneShot(damageClip, 0.1f);

            EventManager.BreakStreak();

            // If the player has lost all it's health and the death flag hasn't been set yet...
            if (currentHealth <= 0 && !PlayerIsDead && !LastchanceAvailable) {
                // Player should die.
                Death();
            } else if (currentHealth <= 0 && !PlayerIsDead && LastchanceAvailable)  //Last chance used
              {
                LastChance();
            }
        }
    }

    public bool PayCost(int cost) {
        if (!(currentHealth < 2 * MaxHealth / 5)) {
            // Reduce the current health by the damage amount.
            currentHealth -= cost;
            return true;
        }
        return false;
    }

    //If returns 0 = Melee, 1 = Utility, 2 = Omega
    public short GetRange() {
        if (currentHealth > 4 * MaxHealth / 5) {
            return 2;
        } else if (currentHealth > 9 * MaxHealth / 20) {
            return 1;
        } else {
            return 0;
        }
    }

    public void Death() {
        // Set the death flag so this function won't be called again.
        PlayerIsDead = true;

        //healthSlider.gameObject.SetActive(false);

        //// Set the audiosource to play the death clip and play it (this will stop the hurt sound from playing).
        //playerAudio.clip = deathClip;
        SoundManager.PlaySFX(deathClip);
        //playerAudio.PlayOneShot(deathClip, 0.1f);
        if (gameObject.name == "TutorialPlayer") {
            //Do nothing. Check point system will happen
        } else if (gameObject.name == "Player 2") {
            ChangeScene("GameOver 2");
        } else if (gameObject.name == "Player 3") {
            ChangeScene("GameOver 3");
        } else
            ChangeScene("GameOver");


    }
    public void ChangeScene(string mSceneName) {
        SceneManager.LoadScene(mSceneName);
    }

    public float CurrentHealth() {
        return currentHealth;
    }

    public float MaximumHealth() {
        return MaxHealth;
    }

    public void LastChance() {
        LastChancePrevBGM = SoundManager.SaveBGM();
        SoundManager.FadeOutBGM();  //fade out current music
        SoundManager.PlaySFX("LastChanceBell");
        LCSoundTimer = LastChanceSoundTime;
        TimeSlowVenue.gameObject.SetActive(true);
        LastResortIcon.color = new Color(LastResortIcon.color.r, LastResortIcon.color.g, LastResortIcon.color.b, .1f);
        lastchanceActive = true;                       //player is in the lastchance state
        LCTimer = LastChanceTime + LastChanceSoundTime;
        Ani.SetTrigger("LastChance");                    //change my animation
        currentHealth = 1;                                    //make sure my player does die on the next frame
        EventManager.EventTime = .25f;
        EventManager.PlayerTime = .7f;
        LastchanceAvailable = false;                              //Cant use last chance 
    }
    void PlaySounds() {
        SoundManager.PlaySFX("ClockTicking");
        SoundManager.PlaySFX("FemaleScream");
        SoundManager.PlaySFX("ManScream");
    }

    void StartDelayedClock() {
        ClockStuff(true);
    }

    void ResetTime() {
        EventManager.EventTime = 1;
        EventManager.PlayerTime = 1;
        ClockStuff(false);
        SoundManager.ReplayBGM(LastChancePrevBGM);
    }

    void EndLastChance() {
        Ani.SetTrigger("Idle");
        lastchanceActive = false;
        PlayerAtt.AttackInProgress = false;
        if (currentHealth <= 400) {
            currentHealth = 400;
        }
        // gameObject.tag = "Player";
    }

    void EventManagerThaw() {
        EventManager.Thaw();
    }

    void ClockStuff(bool _bool) {
        TimeSlowVenue.gameObject.SetActive(_bool);
        LastResortClock.enabled = _bool;
        LastResortClockHand.enabled = _bool;
        AniClockHand.SetBool("LastChance", _bool);
    }

    public void UpdateOmegaNotification(float current, float fade, float length) {
        Color alpha = OmegaUnavailable.color;
        if (current < fade) {
            alpha.a = 1;
        } else {
            alpha.a = 1 - (current - fade) / (length - fade);
        }
        OmegaUnavailable.color = alpha;
    }
}