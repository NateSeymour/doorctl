<template>
  <div class="button" :style="{ background: color }">
    <div class="label"> {{ label  }} </div>
  </div>
</template>

<script setup lang="ts">
const { label } = defineProps<{
  label: string,
  color: string,
}>();
</script>

<style scoped lang="scss">
.button {
  font-size: 40px;
  border-radius: 100%;
  display: grid;
  place-items: center;
  transition: 300ms;
}
</style>