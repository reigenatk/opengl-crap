#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform vec3 lightPos; // world space coords of light
uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    // ambient
    float ambientFactor = 0.05;
    vec3 ambient = ambientFactor * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);

    // get the light vector by taking world space light coords and subtracting pos of fragment
    // then normalizing
    vec3 lightDir = normalize(lightPos - FragPos);

    // If the angle between both vectors is greater than 90 degrees then the result of the dot product
    // will actually become negative and we end up with a negative diffuse component.
    // which kinda makes sense as if light hits a surface at > 90 degrees then
    // it should just be dark since that means its off to the side?
    float diff = max(dot(norm, lightDir), 0.0);

    // now the definition of diffuse- scale the lighting according to this dot product!
    vec3 diffuse = lightColor * diff;

    // specular start
    float specularStrength = 0.5;

    // get the direction from the cam to the fragment
    vec3 viewDir = normalize(viewPos - FragPos);
    // get the reflected light ray
    // negate lightDir cuz without the negative its going from fragment to light
    // which we want the opposite of
    vec3 reflected = reflect(-lightDir, norm);

    // calculate diffuse using dot product
    // dot product will be large when the rays are at similar angles (small theta means cos near 1)
    // and will be near zero when far away
    // the max is there to prevent negative values
    // also the value 32 is configurable, larger = brighter since 
    // the value is less than 1 and we're raising to the xth power
    float amount = pow(max(dot(reflected, viewDir), 0.0), 32);
    vec3 specular = lightColor * specularStrength * amount;

    vec3 finalColor = (diffuse + ambient + specular) * objectColor;
    FragColor = vec4(finalColor, 1.0);
}