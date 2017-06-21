using UnityEngine;
using System.Collections;

public class BaseEnemyMovement : MonoBehaviour {

    public enum AIStates
    {
        Idle = 0,
        Chasing,
        Attacking,
        Dead
    };
    private BaseEnemyAttack AiAttacks;
    private Animator Ani;

    public AIStates mState;
    bool IsStunned = false;
    short StunnedTime = 0;
    [SerializeField]
    Transform mTarget = null;

    //Movement stuff

    public float mMoveSpeed;
    public float mRotationSpeed;
    public float mDistanceKeptAway;      //This is the Distance you want the enemy from the player until he stops moving
    private float mDistanceApart;           //This is the current distance apart from enemy to player          
    public float mEnemyRange;

    BoxCollider2D Collider = null;
   

    



    void Start () {
        //make sure you tag the player with "Player"
        mTarget = GameObject.FindGameObjectWithTag("Player").transform;  //Find the Player and sets my Target Transform.
        AiAttacks = gameObject.GetComponent<BaseEnemyAttack>();
        Ani = gameObject.GetComponent<Animator>();
        mState = AIStates.Idle;
        Collider = GetComponent<BoxCollider2D>();
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

    void BeStunned(short stunnedTime)
    {
        IsStunned = true;
        StunnedTime = stunnedTime;
    }


 
    // Update is called once per frame
    void Update () {
        if(EventManager.IsPaused() || EventManager.IsFrozen() || !Collider.enabled) {
            return;
        }
        if(IsStunned)
        {
            StunnedTime--;
            if(StunnedTime <=0)
            {
                IsStunned = false;
                StunnedTime = 0;
            }
            return;
        }
      
        Vector3 dis = mTarget.position - transform.position;
        mDistanceApart = Vector3.Distance(mTarget.position, transform.position);
        if (mTarget != null)
        {
            switch (mState)
            {
                case AIStates.Idle:

                    if (dis != Vector3.zero && mDistanceApart > mDistanceKeptAway && mDistanceApart <= mEnemyRange)     //if the player close enough to start chasing and not too close to the player;
                    {
                        mState = AIStates.Chasing;
                        Ani.SetTrigger("Chase");
                    }
                    else if (mDistanceApart <= mDistanceKeptAway)       //The enemy is now attacking after he reached his distance.
                    {
                        mState = AIStates.Attacking;                   //Don't set the Animator attack  in here. Do it in player attacks      

                    }
                    break;

                case AIStates.Chasing:

                    if (dis != Vector3.zero && mDistanceApart > mDistanceKeptAway && mDistanceApart <= mEnemyRange)
                    {
                        //Rotate the enemy towards player
                        transform.rotation = Quaternion.Slerp(transform.rotation,
                      Quaternion.FromToRotation(Vector3.right, dis),
                      mRotationSpeed * Time.deltaTime * EventManager.EventTime);

                        //Move towards player
                        transform.position += (mTarget.position - transform.position).normalized
                            * mMoveSpeed * Time.deltaTime * EventManager.EventTime;
                    }
                    else if (mDistanceApart <= mDistanceKeptAway)       //The enemy is now attacking after he reached his distance.
                    {
                        mState = AIStates.Attacking;                    //Don't set the Animator attack  in here. Do it in player attacks           
                       
                    }
                    else if (mDistanceApart > mEnemyRange)      //The player is too far away from the enemy
                    {
                        mState = AIStates.Idle;
                        Ani.SetTrigger("Idle");
                    }
                    break;

                case AIStates.Attacking:
                    {
                        //Rotate the enemy towards player
                        transform.rotation = Quaternion.Slerp(transform.rotation,
                      Quaternion.FromToRotation(Vector3.right, dis),
                      mRotationSpeed * Time.deltaTime * EventManager.EventTime);
                        

                        if ( mDistanceApart > mEnemyRange)      //The player is too far away from the enemy
                        {
                            mState = AIStates.Idle;
                            Ani.SetTrigger("Idle");
                        }
                        else if (AiAttacks.DoesMelee == true && mDistanceApart > mDistanceKeptAway)      //Start chasing if player is furthur than kept away distance (Only The melee should chase right after he gets seprated)
                        {
                            mState = AIStates.Chasing;
                            Ani.SetTrigger("Chase");
                        }
                    }
                    break;
                default:
                    break;
            }
        }
	}

    void OnTriggerStay2D(Collider2D other)
    {


        if (other.gameObject.tag == "Pushing")
        {
            Vector3 directionVector = other.transform.up;

            directionVector.Normalize();
            transform.position = transform.position + directionVector * 0.3f;
        }
    }
    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.tag == "Gathering")
        {
            Vector3 directionVector = -other.transform.up;

            directionVector.Normalize();
            transform.position = transform.position + directionVector * 0.80f;
            IsStunned = true;
            StunnedTime = 30;
        }
    }


    public void informDead() {
        mState = AIStates.Dead;
    }
}
