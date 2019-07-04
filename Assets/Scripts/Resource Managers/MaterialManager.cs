using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using Native.bve.core.image;
using TMPro;
using UnityEngine;
using UnityEngine.Experimental.Rendering;

namespace BVE.Resources {
	class MaterialManager : MonoBehaviour {
		public Material opaqueMaterial;
		public Material cutoutMaterial;
		public Material transparentMaterial;
		
		private Dictionary<Tuple<string, Color32?, Color32>, Material> _cache;

		private Texture GetTexture(string file, Color32? screendoorColor, Color32 multiplyColor) {
			if (!File.Exists(file)) {
				throw new ArgumentException($"Unknown texture {file}");
			}

			using (var loader = new Loader(file)) {
				if (loader.valid() == false) {
					throw new ArgumentException($"Couldn't load image {file}");
				}

				if (screendoorColor != null) {
					var sc = screendoorColor.Value;
					loader.applyScreendoor(sc.r, sc.g, sc.b);
				}

				if (!multiplyColor.Compare(new Color32(255, 255, 255, 255))) {
					loader.multiply(multiplyColor.r, multiplyColor.g, multiplyColor.b, multiplyColor.a);
				}

				var dims = loader.dimensions();

				var tex = new Texture2D(dims.x, dims.y, DefaultFormat.HDR, TextureCreationFlags.MipChain);
				var data = loader.data();
				tex.SetPixels(data);
				tex.Apply();
				return tex;
			}
		}

		public Material GetMaterial(string file, Color32? screendoorColor, Color32 multiplyColor) {
			Material mat;
			if (file.Length != 0) {
				if (multiplyColor.a == 255) {
					mat = Instantiate(transparentMaterial);
				}
				else if (screendoorColor.HasValue && multiplyColor.a == 255) {
					mat = Instantiate(cutoutMaterial);
				}
				else {
					mat = Instantiate(opaqueMaterial);
				}                       
				var tex = GetTexture(file, screendoorColor, multiplyColor);

				mat.mainTexture = tex;
			}

			else {
				mat = Instantiate(opaqueMaterial);
			}

			return mat;
		}

		private void OnDestroy() {
			
		}
	}
}