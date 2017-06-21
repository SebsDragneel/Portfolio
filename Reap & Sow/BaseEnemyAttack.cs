using UnityEngine;
using System.Collections;

public class BaseEnemyAttack : MonoBehaviour {

    public bool DoesShoot;         //Check this if you enemy shoots stuff
    public bool DoesMelee;           //Check this if you enemy melee attacks
    bool Timeslowed = false;
    public GameObject BulletPrefab;
    float bulletTimeStamp = 0.0f;
    float MeleeTimeStamp = 0.0f;
    public float bulletCoolDown = 1.0f;
    public float MeleeCoolDown = 1.0f;
    public float MeleeDamage;

    public string ReloadAudio;                               // The name of the audio clip to play when the player dies.
    bool ReloadState;
    short ReloadTime;
    bool playonce = false;
    public bool alreadyIdle = true;

    float NumOfShots;
    float NumOfPunches;

    float OldAnimationSpeed;
     float NewAnimationSpeed;

    [SerializeField]
    string SFXAttack;

    BaseEnemyMovement States;
    Animator Ani;
    BoxCollider2D Collider;
    HUDBars HUD;

    // Use this for initialization
    void Start () {
        States = GetComponent<BaseEnemyMovement>();
        Ani = GetComponent<Animator>();
        HUD = GameObject.FindGameObjectWithTag("Player").GetComponent<HUDBars>();
        Collider = GetComponent<BoxCollider2D>();
        EventManager.OnPause += Pause;
        EventManager.OnUnpause += Unpause;
        
	}
	
	// Update is called once per frame
	void Update () {
        if(EventManager.IsPaused() || EventManager.IsFrozen() || !Collider.enabled) {
            return;
        }

        if (EventManager.EventTime < 1 && Timeslowed == false)
        {
            Timeslowed = true;
            OldAnimationSpeed = Ani.speed;
            NewAnimationSpeed = EventManager.EventTime * OldAnimationSpeed;
            Ani.speed = NewAnimationSpeed;
        }
        else if (EventManager.EventTime ==1 && Timeslowed == true)
        {
            Ani.speed = OldAnimationSpeed;
        }
        if (!alreadyIdle && States.mState == BaseEnemyMovement.AIStates.Idle)
        {
            Ani.SetTrigger("Idle");
            alreadyIdle = true;
        }
        if (States.mState == BaseEnemyMovement.AIStates.Chasing)
        {
            alreadyIdle = false;        //This is simple to make sure the Idle trigger doesnt constantly get set true;
        }
        if (Ani.GetCurrentAnimatorStateInfo(0).IsName("SoldierAttack"))
        {
            alreadyIdle = false;
        }
        if (States.mState == BaseEnemyMovement.AIStates.Attacking)
        {
            alreadyIdle = false;
            if (DoesShoot == true && Time.time >= bulletTimeStamp)
            {
                bulletTimeStamp = Time.time + bulletCoolDown;
                if (Ani.GetBool("Reload") == false)
                {
                    Ani.SetTrigger("Attack");
                    Shoot();    //Shoot a bullet
                    NumOfShots++;
                    if (NumOfShots % 6 == 5)
                    {
                        Ani.SetBool("Reload", true);
                        ReloadState = true;
                        ReloadTime = 150;
                        playonce = true;
                        return;
                    }
                }
                //  Shoot();    //Shoot a bullet
            }
            if (DoesMelee == true && Time.time >= MeleeTimeStamp)
            {
                MeleeTimeStamp = Time.time + MeleeCoolDown;
                Ani.SetTrigger("Attack");
                NumOfPunches++;
                if (NumOfPunches % 6 == 5)
                {
                    Ani.SetBool("Reload", true);
                    ReloadState = true;
                    ReloadTime = 120;
                    playonce = true;
                }
            }
        }
        if (Ani.GetBool("Reload") == true)
        {
            if (playonce == true)
                SoundManager.PlaySFX(ReloadAudio);

            playonce = false;

        }
        if (ReloadState)
        {
            ReloadTime--;
            if (ReloadTime <= 0)
            {
                ReloadState = false;
                ReloadTime = 0;
                Ani.SetBool("Reload", false);
            }
            return;
        }
    }
    public void Shoot()
    {
        Instantiate(BulletPrefab, transform.position, transform.rotation);
        SoundManager.PlaySFX(SFXAttack);
    }
    public void Attack()        //Call this is your animation 
    {
        HUD.TakeDamage((int)MeleeDamage);
        SoundManager.PlaySFX(SFXAttack);
        
    }
    public void Idle()
    {
        Ani.SetTrigger("Idle");     //This way our AI doesnt get stuck in the attack animation. Even tho he will still be in attack
        
    }

    void Pause() {
        Ani.enabled = false;
    }

    void Unpause() {
        Ani.enabled = true;
    }

    void OnDestroy() {
        EventManager.OnPause -= Pause;
        EventManager.OnUnpause -= Unpause;
    }
}
