#ifdef COMPILEVS
    vec3 GetAmbient(float zonePos)
    {
        return cAmbientStartColor + zonePos * cAmbientEndColor;
    }

    #ifdef NUMVERTEXLIGHTS
        float GetVertexLight(int index, vec3 worldPos, vec3 normal)
        {
            vec3 lightDir = cVertexLights[index * 3 + 1].xyz;
            vec3 lightPos = cVertexLights[index * 3 + 2].xyz;
            float invRange = cVertexLights[index * 3].w;
            float cutoff = cVertexLights[index * 3 + 1].w;
            float invCutoff = cVertexLights[index * 3 + 2].w;

            // Directional light
            if (invRange == 0.0)
            {
                float NdotL = max(dot(normal, lightDir), 0.0);
                return NdotL;
            }
            // Point/spot light
            else
            {
                vec3 lightVec = (lightPos - worldPos) * invRange;
                float lightDist = length(lightVec);
                vec3 localDir = lightVec / lightDist;
                float NdotL = max(dot(normal, localDir), 0.0);
                float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);
                float spotEffect = dot(localDir, lightDir);
                float spotAtten = clamp((spotEffect - cutoff) * invCutoff, 0.0, 1.0);
                return NdotL * atten * spotAtten;
            }
        }

        float GetVertexLightVolumetric(int index, vec3 worldPos)
        {
            vec3 lightDir = cVertexLights[index * 3 + 1].xyz;
            vec3 lightPos = cVertexLights[index * 3 + 2].xyz;
            float invRange = cVertexLights[index * 3].w;
            float cutoff = cVertexLights[index * 3 + 1].w;
            float invCutoff = cVertexLights[index * 3 + 2].w;

            // Directional light
            if (invRange == 0.0)
                return 1.0;
            // Point/spot light
            else
            {
                vec3 lightVec = (lightPos - worldPos) * invRange;
                float lightDist = length(lightVec);
                vec3 localDir = lightVec / lightDist;
                float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);
                float spotEffect = dot(localDir, lightDir);
                float spotAtten = clamp((spotEffect - cutoff) * invCutoff, 0.0, 1.0);
                return atten * spotAtten;
            }
        }
    #endif

    #ifdef SHADOW

        #if defined(DIRLIGHT) && (!defined(GL_ES) || defined(WEBGL))
            #define NUMCASCADES 4
        #else
            #define NUMCASCADES 1
        #endif

        vec4 GetShadowPos(int index, vec4 projWorldPos)
        {
            #if defined(DIRLIGHT)
                return projWorldPos * cLightMatrices[index];
            #elif defined(SPOTLIGHT)
                return projWorldPos * cLightMatrices[1];
            #else
                return vec4(projWorldPos.xyz - cLightPos.xyz, 1.0);
            #endif
        }

    #endif
#endif

#ifdef COMPILEPS
    float GetDiffuse(vec3 normal, vec3 worldPos, out vec3 lightDir)
    {
        #ifdef DIRLIGHT
            lightDir = cLightDirPS;
            return max(dot(normal, lightDir), 0.0);
        #else
            vec3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
            float lightDist = length(lightVec);
            lightDir = lightVec / lightDist;
            return max(dot(normal, lightDir), 0.0) * texture2D(sLightRampMap, vec2(lightDist, 0.0)).r;
        #endif
    }

    float GetDiffuseVolumetric(vec3 worldPos)
    {
        #ifdef DIRLIGHT
            return 1.0;
        #else
            vec3 lightVec = (cLightPosPS.xyz - worldPos) * cLightPosPS.w;
            float lightDist = length(lightVec);
            return texture2D(sLightRampMap, vec2(lightDist, 0.0)).r;
        #endif
    }

    float GetSpecular(vec3 normal, vec3 eyeVec, vec3 lightDir, float specularPower)
    {
        vec3 halfVec = normalize(normalize(eyeVec) + lightDir);
        return pow(max(dot(normal, halfVec), 0.0), specularPower);
    }

    float GetIntensity(vec3 color)
    {
        return dot(color, vec3(0.299, 0.587, 0.114));
    }

    #ifdef SHADOW

        #if defined(DIRLIGHT) && (!defined(GL_ES) || defined(WEBGL))
            #define NUMCASCADES 4
        #else
            #define NUMCASCADES 1
        #endif

        float GetShadow(vec4 shadowPos)
        {
            #ifndef GL_ES
                #ifndef LQSHADOW
                    // Take four samples and average them
                    // Note: in case of sampling a point light cube shadow, we optimize out the w divide as it has already been performed
                    #ifndef POINTLIGHT
                        vec2 offsets = cShadowMapInvSize * shadowPos.w;
                    #else
                        vec2 offsets = cShadowMapInvSize;
                    #endif
                    #ifndef GL3
                        return cShadowIntensity.y + cShadowIntensity.x * (shadow2DProj(sShadowMap, shadowPos).r +
                            shadow2DProj(sShadowMap, vec4(shadowPos.x + offsets.x, shadowPos.yzw)).r +
                            shadow2DProj(sShadowMap, vec4(shadowPos.x, shadowPos.y + offsets.y, shadowPos.zw)).r +
                            shadow2DProj(sShadowMap, vec4(shadowPos.xy + offsets.xy, shadowPos.zw)).r);
                    #else
                        return cShadowIntensity.y + cShadowIntensity.x * (textureProj(sShadowMap, shadowPos) +
                            textureProj(sShadowMap, vec4(shadowPos.x + offsets.x, shadowPos.yzw)) +
                            textureProj(sShadowMap, vec4(shadowPos.x, shadowPos.y + offsets.y, shadowPos.zw)) +
                            textureProj(sShadowMap, vec4(shadowPos.xy + offsets.xy, shadowPos.zw)));
                    #endif
                #else
                    // Take one sample
                    #ifndef GL3
                        float inLight = shadow2DProj(sShadowMap, shadowPos).r;
                    #else
                        float inLight = textureProj(sShadowMap, shadowPos);
                    #endif
                    return cShadowIntensity.y + cShadowIntensity.x * inLight;
                #endif
            #else
                #ifndef LQSHADOW
                    // Take four samples and average them
                    vec2 offsets = cShadowMapInvSize * shadowPos.w;
                    vec4 inLight = vec4(
                        texture2DProj(sShadowMap, shadowPos).r * shadowPos.w > shadowPos.z,
                        texture2DProj(sShadowMap, vec4(shadowPos.x + offsets.x, shadowPos.yzw)).r * shadowPos.w > shadowPos.z,
                        texture2DProj(sShadowMap, vec4(shadowPos.x, shadowPos.y + offsets.y, shadowPos.zw)).r * shadowPos.w > shadowPos.z,
                        texture2DProj(sShadowMap, vec4(shadowPos.xy + offsets.xy, shadowPos.zw)).r * shadowPos.w > shadowPos.z
                    );
                    return cShadowIntensity.y + dot(inLight, vec4(cShadowIntensity.x));
                #else
                    // Take one sample
                    return cShadowIntensity.y + (texture2DProj(sShadowMap, shadowPos).r * shadowPos.w > shadowPos.z ? cShadowIntensity.x : 0.0);
                #endif
            #endif
        }

        #ifdef POINTLIGHT
            float GetPointShadow(vec3 lightVec)
            {
                vec3 axis = textureCube(sFaceSelectCubeMap, lightVec).rgb;
                float depth = abs(dot(lightVec, axis));

                // Expand the maximum component of the light vector to get full 0.0 - 1.0 UV range from the cube map,
                // and to avoid sampling across faces. Some GPU's filter across faces, while others do not, and in this
                // case filtering across faces is wrong
                const vec3 factor = vec3(1.0 / 256.0);
                lightVec += factor * axis * lightVec;

                // Read the 2D UV coordinates, adjust according to shadow map size and add face offset
                vec4 indirectPos = textureCube(sIndirectionCubeMap, lightVec);
                indirectPos.xy *= cShadowCubeAdjust.xy;
                indirectPos.xy += vec2(cShadowCubeAdjust.z + indirectPos.z * 0.5, cShadowCubeAdjust.w + indirectPos.w);

                vec4 shadowPos = vec4(indirectPos.xy, cShadowDepthFade.x + cShadowDepthFade.y / depth, 1.0);
                return GetShadow(shadowPos);
            }
        #endif

        #ifdef DIRLIGHT
            float GetDirShadowFade(float inLight, float depth)
            {
                return min(inLight + max((depth - cShadowDepthFade.z) * cShadowDepthFade.w, 0.0), 1.0);
            }

            #if !defined(GL_ES) || defined(WEBGL)
                float GetDirShadow(const vec4 iShadowPos[NUMCASCADES], float depth)
                {
                    vec4 shadowPos;

                    if (depth < cShadowSplits.x)
                        shadowPos = iShadowPos[0];
                    else if (depth < cShadowSplits.y)
                        shadowPos = iShadowPos[1];
                    else if (depth < cShadowSplits.z)
                        shadowPos = iShadowPos[2];
                    else
                        shadowPos = iShadowPos[3];
                        
                    return GetDirShadowFade(GetShadow(shadowPos), depth);
                }
            #else
                float GetDirShadow(const vec4 iShadowPos[NUMCASCADES], float depth)
                {
                    return GetDirShadowFade(GetShadow(iShadowPos[0]), depth);
                }
            #endif

            #ifndef GL_ES
                float GetDirShadowDeferred(vec4 projWorldPos, float depth)
                {
                    vec4 shadowPos;

                    if (depth < cShadowSplits.x)
                        shadowPos = projWorldPos * cLightMatricesPS[0];
                    else if (depth < cShadowSplits.y)
                        shadowPos = projWorldPos * cLightMatricesPS[1];
                    else if (depth < cShadowSplits.z)
                        shadowPos = projWorldPos * cLightMatricesPS[2];
                    else
                        shadowPos = projWorldPos * cLightMatricesPS[3];

                    return GetDirShadowFade(GetShadow(shadowPos), depth);
                }
            #endif
        #endif

        float GetShadow(vec4 iShadowPos[NUMCASCADES], float depth)
        {
            #if defined(DIRLIGHT)
                return GetDirShadow(iShadowPos, depth);
            #elif defined(SPOTLIGHT)
                return GetShadow(iShadowPos[0]);
            #else
                return GetPointShadow(iShadowPos[0].xyz);
            #endif
        }

        #ifndef GL_ES
            float GetShadowDeferred(vec4 projWorldPos, float depth)
            {
                #if defined(DIRLIGHT)
                    return GetDirShadowDeferred(projWorldPos, depth);
                #elif defined(SPOTLIGHT)
                    vec4 shadowPos = projWorldPos * cLightMatricesPS[1];
                    return GetShadow(shadowPos);
                #else
                    vec3 shadowPos = projWorldPos.xyz - cLightPosPS.xyz;
                    return GetPointShadow(shadowPos);
                #endif
            }
        #endif
    #endif
    
    #ifdef PBR
    
        vec3 LinearToSRGB(vec3 c)
        {
            float power = 2.2;
            return pow(c, vec3(power, power, power));
        }

        vec3 LinearFromSRGB(vec3 c)
        {
            float power = 1.0 / 2.2;
            return pow(c, vec3(power, power, power));
        }
    
        /// Diffuse factors

        /// Oren-Nayar diffuse factor
        ///     diffuseColor: input rgb
        ///     roughness: roughness of the surface
        ///     nDotV: dot prod of surface normal and view direction
        ///     nDotL: dot prod of surface normal and light direction
        ///     vDotH: dot prod of view direction and half-angle
        vec3 OrenNayarDiffuse(in vec3 diffuseColor, in float roughness, in float nDotV, in float nDotL, in float vDotH)
        {
            float rough2 = roughness * roughness;
            
            float vDotL = 2.0 * vDotH - 1.0;
            float majorCtrl = 1.0 - 0.5 * rough2 / (rough2 + 0.33);
            float cosRi = vDotL - nDotV * nDotL;
            float minorCtrl = 0.4545 * rough2 / (rough2 + 0.09) * cosRi * (cosRi >= 0.0 ? min(1.0, nDotL / nDotV) : nDotL);
            return diffuseColor / 3.141596 * (nDotL * majorCtrl + minorCtrl);
        }

        /// Lambertian diffuse factor
        ///     diffuseColor: input rgb
        ///     roughness: roughness of the surface
        ///     nDotV: dot prod of surface normal and view direction
        ///     nDotL: dot prod of surface normal and light direction
        ///     vDotH: dot prod of view direction and half-angle
        vec3 LambertianDiffuse(in vec3 diffuseColor, in float roughness, in float nDotV, in float nDotL, in float vDotH)
        {
            return diffuseColor * nDotL;
        }
        
    /// Fresnel Terms

        /// Fresnel factor
        ///     specular: Specular color input
        ///     vDotH: dot product of view direction and half-angle
        vec3 SchlickFresnel(in vec3 specular, in float vDotH)
        {
            return specular + (vec3(1.0, 1.0, 1.0) - specular) * pow(1.0 - vDotH, 5.0);
        }
        
        /// Fresnel factor, spherical gaussian in Schlick approximation; https://seblagarde.wordpress.com/2012/06/03/spherical-gaussien-approximation-for-blinn-phong-phong-and-fresnel/
        ///     specular: specular color of the surface
        ///     vDotH: dot product of view direction and half-angle
        vec3 SchlickGaussianFresnel(in vec3 specular, in float vDotH)
        {
            float sphericalGaussian = pow(2.0, (-5.55473 * vDotH - 6.98316) * vDotH);
            return specular + (vec3(1.0, 1.0, 1.0) - specular) * sphericalGaussian;
        }
        
    // Visibility terms

        /// Smith GGX Visibility
        ///     nDotL: dot-prod of surface normal and light direction
        ///     nDotV: dot-prod of surface normal and view direction
        ///     roughness: surface roughness
        float SmithGGXVisibility(in float nDotL, in float nDotV, in float roughness)
        {
            float rough2 = roughness * roughness;
            float gSmithV = nDotV + sqrt(nDotV * (nDotV - nDotV * rough2) + rough2);
            float gSmithL = nDotL + sqrt(nDotL * (nDotL - nDotL * rough2) + rough2);
            return 1.0 / ( gSmithV * gSmithL );
        }
        
        
        float SchlickG1(in float factor, in float rough2)
        {
            return 1.0 / (factor * (1.0 - rough2) + rough2);
        }
        
        /// Schlick approximation of Smith GGX
        ///     nDotL: dot product of surface normal and light direction
        ///     nDotV: dot product of surface normal and view direction
        ///     roughness: surface roughness
        float SchlickVisibility(float nDotL, float nDotV, float roughness)
        {
            float rough2 = roughness * roughness;
            return (SchlickG1(nDotL, rough2) * SchlickG1(nDotV, rough2)) * 0.25; // divided by four
        }
        
    /// Normal Distributions
        
        /// Beckmann normal distribution
        ///     nDotH: dot-prod of surface normal and half-angle
        ///     roughness: surface roughness
        float BeckmannDistribution(in float nDotH, in float roughness)
        {
            float rough2 = roughness * roughness;
            float roughnessA = 1.0 / (4.0 * rough2 * pow(nDotH, 4.0));
            float roughnessB = nDotH * nDotH - 1.0;
            float roughnessC = rough2 * nDotH * nDotH;
            return roughnessA * exp(roughnessB / roughnessC);
        }
        
        /// Trowbridge-Reitz GGX normal distribution
        ///     nDotH: dot-prod of surface normal and half-angle
        ///     roughness: surface roughness
        float GGXDistribution(in float nDotH, in float roughness)
        {
            float rough2 = roughness * roughness;
            float tmp = roughness / max(1e-8, nDotH * nDotH * (rough2 - 1.0) + 1.0);
            return tmp * tmp * 0.3141596;
        }
        
    /// Functions

        /// BRDF evaluation for direct light
        ///     vDotH: dot product of view and half vectors
        ///     nDotL: dot product of surface normal and light vectors
        ///     nDotV: dot product of surface normal and view vector
        ///     nDotH: dot product of surface normal and half vector
        ///     specular: specular color
        ///     roughness: surface roughness
        ///     fresnel: fresnel intensity at 90 degrees
        vec3 BRDF(in float vDotH, in float nDotL, in float nDotV, in float nDotH, in vec3 specular, in float roughness)
        {
            return SchlickFresnel(specular, vDotH) *
                (GGXDistribution(nDotH, roughness) * SmithGGXVisibility(nDotL, nDotV, roughness) / 4.0);
        }

        /// Light contribution
        ///     vDotH: dot product of view and half vectors
        ///     nDotL: dot product of surface normal and light vectors
        ///     nDotV: dot product of surface normal and view vector
        ///     nDotH: dot product of surface normal and half vector
        ///     diffuse: diffuse color
        ///     specular: specular color
        ///     roughness: sampled roughness value
        ///     lightcolor: color of light APPLY ANY ATTENUATION FIRST
        ///     lightIntensity: intensity of the light contribution APPLY ANY ATTENUATION FIRST
        vec3 LightContribution(in float vDotH, in float nDotL, in float nDotV, in float nDotH, in vec3 diffuse, in vec3 specular, in float roughness, in vec3 lightColor, in float lightIntensity)
        {    
            return BRDF(vDotH, nDotL, nDotV, nDotH, specular, roughness) * lightIntensity;
        }

        
        #ifdef IBL      /// Image based lighting shading

            /// Epic's approximation, convenient outside of mobile as well - very tolerant of 'lazy' IBL such as unfiltered mips
            /// https://www.unrealengine.com/blog/physically-based-shading-on-mobile
            ///     specColor: specular color of the fragment
            ///     roughness: surface roughness
            ///     nDotV: dot product of normal and view vectors
            vec3 EnvBRDFApprox(in vec3 specColor, in float roughness, in float nDotV )
            {            
                vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
                vec4 c1 = vec4(1.0, 0.0425, 1.0, -0.04 );
                vec4 r = roughness * c0 + c1;
                float a004 = min( r.x * r.x, exp2( -9.28 * nDotV)) * r.x + r.y;
                vec2 AB = vec2( -1.04, 1.04) * a004 + r.zw;
                return specColor * AB.x + AB.y;
            }

            /// Determine reflection vector based on surface roughness, rougher uses closer to the normal and smoother uses closer to the reflection vector
            ///     normal: surface normal
            ///     reflection: vector of reflection off of the surface
            ///     roughness: surface roughness
            vec3 GetSpecularDominantDir(vec3 normal, vec3 reflection, float roughness) 
            { 
                float smoothness = 1.0 - roughness; 
                float lerpFactor = smoothness * (sqrt(smoothness) + roughness); 
                return mix(normal, reflection, lerpFactor);
            }
            
            /// Calculate IBL contributation
            ///     reflectVec: reflection vector for cube sampling
            ///     wsNormal: surface normal in word space
            ///     toCamera: normalized direction from surface point to camera
            ///     roughness: surface roughness
            ///     ambientOcclusion: ambient occlusion
            vec3 ImageBasedLighting(in vec3 reflectVec, in vec3 wsNormal, in vec3 toCamera, in vec3 specular, in float roughness, out vec3 reflectionCubeColor)
            {    
                reflectVec = GetSpecularDominantDir(wsNormal, reflectVec, roughness);
                float ndv = abs(dot(wsNormal, toCamera));
                
                // Mip selection is something to tune to your desired results
                //float mipSelect = 7.0 - 1.0 + log2(roughness); // Geilfus: https://github.com/simongeilfus/Cinder-Experiments
                float mipSelect = roughness * 9.0;  // Lux-style
                
                reflectionCubeColor.rgb = textureCube(sZoneCubeMap, reflectVec, 9).rgb;
                vec3 specularCube = textureCube(sZoneCubeMap, reflectVec, mipSelect).rgb;
                vec3 environmentSpecular = EnvBRDFApprox(specular, roughness, ndv);
                return specularCube * environmentSpecular;
            }

        #endif
    #endif
#endif
