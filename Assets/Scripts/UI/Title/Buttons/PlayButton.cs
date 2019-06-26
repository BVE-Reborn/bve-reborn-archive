using UnityEngine;

namespace BVE.UI.Title.Buttons {
    public class PlayButton : MonoBehaviour {
        public GameObject dialogue;

        public void Click() {
            dialogue.SetActive(true);
        }
    }
}