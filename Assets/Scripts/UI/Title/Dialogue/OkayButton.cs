using UnityEngine;

namespace BVE.UI.Title.Dialogue {
    public class OkayButton : MonoBehaviour {
        public GameObject dialogue;

        public void Click(){
            dialogue.SetActive(false);
        }
    }
}
