using UnityEngine;
using System.Collections;

public class ObstacleCollisions : MonoBehaviour {

	void OnTriggerEnter2D(Collider2D collision) {
        if(collision.GetComponent<Bullet>() != null) {
            Destroy(collision.gameObject);
        }
    }
}
