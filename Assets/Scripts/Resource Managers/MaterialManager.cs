using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using JetBrains.Annotations;
using Native.bve.core.image;
using TMPro;
using UnityEngine;
using UnityEngine.Experimental.Rendering;

namespace BVE.Resources {
    class MaterialManager : MonoBehaviour {
        public Material opaqueMaterial;
        public Material cutoutMaterial;
        public Material transparentMaterial;

        private struct MaterialContainer {
            public long refCount;
            public readonly Material material;

            public MaterialContainer(long refCount, [NotNull] Material material) {
                this.refCount = refCount;
                this.material = material ?? throw new ArgumentNullException(nameof(material));
            }
        }

        private readonly Dictionary<Tuple<string, Color32?, Color32>, MaterialContainer> _cache = new Dictionary<Tuple<string, Color32?, Color32>, MaterialContainer>();

        private static Texture GetTexture(string file, Color32? screendoorColor, Color32 multiplyColor) {
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

        private static Texture2D GetSingleColorTexture(Color32 color) {
            var tex = new Texture2D(1, 1, DefaultFormat.LDR, TextureCreationFlags.MipChain);
            tex.SetPixel(0, 0, color);
            tex.Apply();
            return tex;
        }

        private Material GetMaterialImpl(string file, Color32? screendoorColor, Color32 multiplyColor) {
            Material mat;
            Texture tex;
            if (file.Length != 0) {
                if (multiplyColor.a != 255) {
                    mat = Instantiate(transparentMaterial);
                }
                else if (screendoorColor.HasValue) {
                    mat = Instantiate(cutoutMaterial);
                }
                else {
                    mat = Instantiate(opaqueMaterial);
                }

                tex = GetTexture(file, screendoorColor, multiplyColor);
            }
            else {
                if (multiplyColor.a != 255) {
                    mat = Instantiate(transparentMaterial);
                }
                else {
                    mat = Instantiate(opaqueMaterial);
                }

                tex = GetSingleColorTexture(multiplyColor);
            }

            mat.mainTexture = tex;

            return mat;
        }

        public Material GetMaterial(string file, Color32? screendoorColor, Color32 multiplyColor) {
            var argTuple = new Tuple<string, Color32?, Color32>(file, screendoorColor, multiplyColor);

            if (_cache.TryGetValue(argTuple, out MaterialContainer matContainer)) {
                matContainer.refCount += 1;
                return matContainer.material;
            }

            var material = GetMaterialImpl(file, screendoorColor, multiplyColor);
            matContainer = new MaterialContainer(1, material);
            _cache.Add(argTuple, matContainer);

            return matContainer.material;
        }

        public void RemoveMaterial(string file, Color32? screendoorColor, Color32 multiplyColor) {
            var argTuple = new Tuple<string, Color32?, Color32>(file, screendoorColor, multiplyColor);
            
            if (!_cache.TryGetValue(argTuple, out MaterialContainer matContainer)) {
                throw new ArgumentException($"Double free of texture {file}:{screendoorColor}:{multiplyColor}");
            }

            var count = matContainer.refCount -= 1;
            if (count == 0) {
                Destroy(matContainer.material);
                _cache.Remove(argTuple);
            }
        }


        private void OnDestroy() {
            if (_cache.Count != 0) {
                Debug.LogError("All materials not freed");
            }
        }
    }
}