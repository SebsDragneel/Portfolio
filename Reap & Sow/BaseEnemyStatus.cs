using UnityEngine;
using System.Collections;
using UnityEngine.SceneManagement;


public class BaseEnemyStatus : MonoBehaviour {
    Animator Ani;
    public GameObject mBloodParticle = null; 
    public GameObject Blood = null;
    public float Maxhealth = 100;
    public float Currenthealth;
    private CameraController Shake = null;

    //Sounds
    [SerializeField]
    string SFXHit = "nullsound";
    [SerializeField]
    string SFXDeath = "nullsound";
    [SerializeField]
    string SFXBlood = "nullsound";

    //SCORE/MULTIPLIER EMP
    [SerializeField]
    int scoreValue = 30;
    //Need a refrence of the Player so 


    //Slot for prefab of soul
    [SerializeField]
    GameObject Soul = null;
    [SerializeField]
    GameObject bullet = null;

    //Number of souls they drop
    [SerializeField]
    float soulCount = 1;
    bool alreadyDead = false;
    public bool fireDmg = true;
    //Power Up drops
    DeathDropManager dropper;
    [SerializeField]
    bool ChanceToDropPowerups = true;
    public string FireHitSound = "nullsound", FireDeathSound = "nullsound";


    // Use this for initialization
    void Start() {
        Physics2D.IgnoreCollision(GetComponent<Collider2D>(), bullet.GetComponent<Collider2D>());
        Currenthealth = Maxhealth;
        Ani = GetComponent<Animator>();
        Shake = Camera.main.gameObject.GetComponent<CameraController>();
        if (ChanceToDropPowerups) {
            dropper = GetComponent<DeathDropManager>();
        }
        EventManager.OnPause += Pause;
        EventManager.OnUnpause += Unpause;
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

    // Update is called once per frame
    void Update() {
        if(EventManager.IsPaused()) {
            return;
        }
        if (Currenthealth <= 0 && !alreadyDead) {
            Ani.SetTrigger("Death");
            alreadyDead = true;
        }
    }

    public void TakeDamage(float _dmg) {
        if (!alreadyDead) {
            if (Currenthealth - _dmg <= 0) {
                Currenthealth = 0;
                SoundManager.PlaySFX(SFXDeath);
            } else if(hasReload() && Ani.GetBool("Reload") == true) {

                Currenthealth -= _dmg * 2;
                Instantiate(mBloodParticle, transform.position, transform.rotation);
                SoundManager.PlaySFX(SFXBlood);
                Instantiate(Blood, transform.position, transform.rotation);
                Shake.DoShakeCamera(1, 1);


            } else {
                Currenthealth -= _dmg;
                if(mBloodParticle) {
                    Instantiate(mBloodParticle, transform.position, transform.rotation);
                }
                if (name.Contains("Witch"))
                {
                    SoundManager.PlaySFX("WitchHit");
                }
                else
                SoundManager.PlaySFX(SFXBlood);

            }
        }
    }

    public void TakeLastChanceDamage(float _dmg)           //only when the player is in last chance 
    {
        if (alreadyDead) {
            return;
        }
        if (Currenthealth - _dmg <= 0) {
            SoundManager.PlaySFX("SoldierDisintegration");
            Ani.SetTrigger("Fade");
            alreadyDead = true;
        } else
            Currenthealth -= _dmg;
    }

    public void TakeAbsorbDamage(float _dmg)           //only when the player is in last chance 
    {
        if (alreadyDead) {
            return;
        }
        if (Currenthealth - _dmg <= 0) {
            SoundManager.PlaySFX("SoldierDisintegration");
            Ani.SetTrigger("Fade");
            soulCount += 5;
            alreadyDead = true;
        } else
            Currenthealth -= _dmg;
    }

    public void DestroyME() {
        //SCORE/MULTIPLIER 
        if (!fireDmg)
        {
            EventManager.EnemyDied(scoreValue);
        }
        GameObject powerUp;

        for (float i = 0; i < soulCount; i++)
            Instantiate(Soul, new Vector3(transform.position.x + (Random.Range(-1, 2) * (i / 30)), transform.position.y + (Random.Range(-1, 2) * (i / 30)), 0), new Quaternion());

        if (dropper && ChanceToDropPowerups) {
            powerUp = dropper.Drop(Random.Range(20, 662));
            if (powerUp != null) {
                Instantiate(powerUp, transform.position, new Quaternion());
            }
        }

        if(gameObject.name == "3rdBoss" )
        {
            Destroy(gameObject);
            ChangeScene("Win 3");
        }
        //else if(gameObject.name == "Tank")
        //{
        //    Destroy(gameObject);
        //    ChangeScene("Win 2");
        //}

        Destroy(gameObject);
    }

    public void ChangeScene(string mSceneName)
    {
        SceneManager.LoadScene(mSceneName);
    }

    bool hasReload() {
        foreach(AnimatorControllerParameter p in Ani.parameters) {
            if(p.name == "Reload") {
                return true;
            }
        }
        return false;
    }
}


